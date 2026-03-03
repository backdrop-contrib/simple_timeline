<?php
/**
 * @file
 * Template for the Enhanced Timeline Views style plugin.
 *
 * Variables:
 * - $view: The view object.
 * - $options: Style plugin options array.
 * - $rows: Rendered row output, array of HTML strings keyed by row index.
 * - $timestamps: Unix timestamps keyed by row index.
 */

$layout = isset($options['layout']) ? $options['layout'] : 'alternating';
?>
<div class="enhanced-timeline-container enhanced-timeline-layout-<?php print check_plain($layout); ?>">
  <div class="enhanced-timeline-line"></div>
  <?php foreach ($rows as $i => $row): ?>
    <?php
      switch ($layout) {
        case 'right': $side = 'right'; break;
        case 'left':  $side = 'left';  break;
        default:      $side = ($i % 2 === 0) ? 'right' : 'left';
      }
    ?>
    <div class="enhanced-timeline-event enhanced-timeline-event-<?php print $side; ?>"
         data-timestamp="<?php print isset($timestamps[$i]) ? (int) $timestamps[$i] : 0; ?>">
      <div class="enhanced-timeline-dot"></div>
      <div class="enhanced-timeline-stem"></div>
      <div class="enhanced-timeline-card">
        <?php print $row; ?>
      </div>
    </div>
  <?php endforeach; ?>
</div>
