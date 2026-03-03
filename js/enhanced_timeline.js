/**
 * @file
 * Proportional positioning for the Enhanced Timeline Views style plugin.
 */
(function ($) {
  Backdrop.behaviors.enhancedTimeline = {
    attach: function (context, settings) {
      var cfg = settings.enhancedTimeline || {};
      var minHeight = cfg.minHeight || 600;
      var heightPerYear = cfg.heightPerYear || 200;
      var SECS_PER_YEAR = 365.25 * 24 * 3600;
      var MIN_GAP = 20;
      var MOBILE_BREAKPOINT = 640;
      // Dot natural offset within event element — matches .enhanced-timeline-dot { top: 20px }.
      // Dot center = DOT_CSS_TOP + 7 (half of 14px dot height) = 27px.
      var DOT_CSS_TOP = 20;
      // Center of ::before connector — matches { top: 26px; height: 2px } → 27px.
      var CONNECTOR_CTR = 27;

      $('.enhanced-timeline-container', context).once('enhanced-timeline-init', function () {
        var $container = $(this);
        var $events = $container.find('.enhanced-timeline-event[data-timestamp]');
        if ($events.length < 2) { return; }

        var timestamps = [];
        $events.each(function () {
          timestamps.push(parseInt($(this).data('timestamp'), 10));
        });

        // ----------------------------------------------------------------
        // Build column groups (same logic for both spacing modes).
        // ----------------------------------------------------------------
        function buildGroups(isMobile) {
          var layout = cfg.layout || 'alternating';
          if (isMobile || layout === 'right' || layout === 'left') {
            var all = [];
            $events.each(function (i) { all.push({ i: i, $el: $(this) }); });
            return [all];
          }
          var right = [], left = [];
          $events.each(function (i) {
            ($(this).hasClass('enhanced-timeline-event-right') ? right : left)
              .push({ i: i, $el: $(this) });
          });
          return [right, left];
        }

        // ----------------------------------------------------------------
        // Grow the container to fit all positioned cards.
        // ----------------------------------------------------------------
        function expandContainer(padding) {
          var maxBottom = 0;
          $events.each(function () {
            var bottom = $(this).position().top + $(this).outerHeight(true);
            if (bottom > maxBottom) { maxBottom = bottom; }
          });
          $container.css('min-height', (maxBottom + padding) + 'px');
        }

        // ----------------------------------------------------------------
        // Proportional spacing.
        //
        // Dot positions: proportional to time, then nudged as one column
        //   so every dot is individually visible (18 px min gap).
        // Card positions: start from ideal dot positions, nudged per
        //   side-column to eliminate card overlaps.
        // A vertical stem bridges any gap between dot and card.
        // ----------------------------------------------------------------
        function applyProportionalSpacing(isMobile) {
          var minTs = Math.min.apply(null, timestamps);
          var maxTs = Math.max.apply(null, timestamps);
          var range = maxTs - minTs;
          if (range === 0) { return; }

          var years = range / SECS_PER_YEAR;
          var usableHeight = Math.max(minHeight, Math.ceil(years * heightPerYear));
          var padding = 80;
          $container.css('min-height', usableHeight + 'px');

          // Ideal (proportional) dot positions.
          var dotPos = [];
          $events.each(function (i) {
            var fraction = (timestamps[i] - minTs) / range;
            dotPos[i] = padding + Math.round(fraction * (usableHeight - padding * 2));
          });

          // Nudge card positions per side-column to eliminate card overlaps.
          var cardPos = dotPos.slice();
          buildGroups(isMobile).forEach(function (group) {
            group.sort(function (a, b) { return dotPos[a.i] - dotPos[b.i]; });
            var minNext = 0;
            group.forEach(function (ev) {
              cardPos[ev.i] = Math.max(dotPos[ev.i], minNext);
              ev.$el.css('top', cardPos[ev.i] + 'px');
              minNext = cardPos[ev.i] + ev.$el.outerHeight(true) + MIN_GAP;
            });
          });

          // Nudge dot positions so every dot is individually visible on the
          // center line (all dots share the same line regardless of card side).
          var MIN_DOT_GAP = 18;
          var dotNudged = dotPos.slice();
          var sortedIdx = [];
          $events.each(function (i) { sortedIdx.push(i); });
          sortedIdx.sort(function (a, b) { return dotPos[a] - dotPos[b]; });
          var minNextDot = 0;
          sortedIdx.forEach(function (i) {
            dotNudged[i] = Math.max(dotPos[i], minNextDot);
            minNextDot = dotNudged[i] + MIN_DOT_GAP;
          });

          // Position dot and stem relative to each card.
          //
          // When dotNudged[i] === cardPos[i] (no displacement), dotTopPx = DOT_CSS_TOP
          // and dotCtrPx = CONNECTOR_CTR, so stemHeight = 0 → no stem, straight line.
          // When displaced, the stem bridges dot centre to connector centre.
          $events.each(function (i) {
            var offset   = dotNudged[i] - cardPos[i];
            var dotTopPx = DOT_CSS_TOP + offset;
            var dotCtrPx = dotTopPx + 7; // 7 = half of 14px dot height
            var $dot  = $(this).find('.enhanced-timeline-dot');
            var $stem = $(this).find('.enhanced-timeline-stem');

            $dot.css('top', dotTopPx + 'px');

            var stemHeight = Math.abs(dotCtrPx - CONNECTOR_CTR);
            if (stemHeight > 1) {
              $stem.css({
                top:    Math.min(dotCtrPx, CONNECTOR_CTR) + 'px',
                height: stemHeight + 'px'
              }).show();
            } else {
              $stem.hide();
            }
          });

          expandContainer(padding);
        }

        // ----------------------------------------------------------------
        // Equal spacing — stack cards with a fixed gap, no time maths.
        // Dot sits at a fixed offset inside each card; no stem needed.
        // ----------------------------------------------------------------
        function applyEqualSpacing(isMobile) {
          var padding = 80;
          buildGroups(isMobile).forEach(function (group) {
            var nextTop = padding;
            group.forEach(function (ev) {
              ev.$el.css('top', nextTop + 'px');
              ev.$el.find('.enhanced-timeline-dot').css('top', ''); // reset to CSS default
              ev.$el.find('.enhanced-timeline-stem').hide();
              nextTop += ev.$el.outerHeight(true) + MIN_GAP;
            });
          });
          expandContainer(padding);
        }

        // ----------------------------------------------------------------
        // Main entry point.
        // ----------------------------------------------------------------
        function positionEvents() {
          var isMobile = $container.width() <= MOBILE_BREAKPOINT;
          if (cfg.spacing === 'equal') {
            applyEqualSpacing(isMobile);
          } else {
            applyProportionalSpacing(isMobile);
          }
        }

        positionEvents();

        var resizeTimer;
        $(window).on('resize.enhanced-timeline', function () {
          clearTimeout(resizeTimer);
          resizeTimer = setTimeout(positionEvents, 100);
        });
      });

      // Tap / click a card to raise it above any overlapping neighbour.
      $('.enhanced-timeline-container .enhanced-timeline-event', context)
        .on('click.enhanced-timeline touchstart.enhanced-timeline', function (e) {
          var $this = $(this);
          $this.closest('.enhanced-timeline-container')
               .find('.enhanced-timeline-event').removeClass('enhanced-timeline-event--active');
          $this.addClass('enhanced-timeline-event--active');
          e.stopPropagation();
        });

      $(document).on('click.enhanced-timeline touchstart.enhanced-timeline', function () {
        $('.enhanced-timeline-event--active').removeClass('enhanced-timeline-event--active');
      });
    }
  };
}(jQuery));
