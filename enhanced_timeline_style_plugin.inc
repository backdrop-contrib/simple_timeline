<?php
/**
 * @file
 * Style plugin for the Enhanced Timeline Views display.
 */

/**
 * Style plugin to render results as a proportionally-spaced vertical timeline.
 *
 * @ingroup views_style_plugins
 */
class enhanced_timeline_style_plugin extends views_plugin_style {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['date_source'] = array('default' => 'created');
    $options['date_field'] = array('default' => '');
    $options['layout'] = array('default' => 'alternating');
    $options['spacing'] = array('default' => 'proportional');
    $options['min_height'] = array('default' => 600);
    $options['height_per_year'] = array('default' => 200);
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['date_source'] = array(
      '#type' => 'select',
      '#title' => t('Date source'),
      '#default_value' => $this->options['date_source'],
      '#options' => array(
        'created' => t('Node created date'),
        'field'   => t('Custom date field (requires Date module)'),
      ),
      '#description' => t('The date used to position events on the timeline. "Node created date" works with any content type with no extra configuration.'),
    );

    $form['date_field'] = array(
      '#type' => 'textfield',
      '#title' => t('Date field machine name'),
      '#default_value' => $this->options['date_field'],
      '#description' => t('Machine name of a date field to use for positioning (e.g. <em>field_event_date</em>). Only used when "Custom date field" is selected above.'),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[date_source]"]' => array('value' => 'field'),
        ),
      ),
    );

    $form['layout'] = array(
      '#type' => 'select',
      '#title' => t('Event layout'),
      '#default_value' => $this->options['layout'],
      '#options' => array(
        'alternating' => t('Alternating left/right'),
        'right'       => t('All on the right'),
        'left'        => t('All on the left'),
      ),
      '#description' => t('How events are placed relative to the centre line.'),
    );

    $form['spacing'] = array(
      '#type' => 'select',
      '#title' => t('Event spacing'),
      '#default_value' => $this->options['spacing'],
      '#options' => array(
        'proportional' => t('Proportional to time gaps'),
        'equal'        => t('Equal — stack evenly regardless of time gaps'),
      ),
      '#description' => t('Proportional spacing reflects the real time between events. Equal spacing stacks cards with a consistent gap so no space is wasted.'),
    );

    $form['min_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum container height (px)'),
      '#default_value' => $this->options['min_height'],
      '#description' => t('Minimum height of the timeline container in pixels.'),
    );

    $form['height_per_year'] = array(
      '#type' => 'textfield',
      '#title' => t('Height per year (px)'),
      '#default_value' => $this->options['height_per_year'],
      '#description' => t('Pixels of height allocated per year of time span. Only applies to proportional spacing.'),
    );
  }

  /**
   * {@inheritdoc}
   */
  function render() {
    // Pre-render all fields so rendered_fields is populated before row
    // plugins call get_field() mid-loop.
    $this->render_fields($this->view->result);

    // Render all rows via the row plugin.
    $rows = array();
    foreach ($this->view->result as $i => $row) {
      $this->view->row_index = $i;
      $rows[] = $this->row_plugin->render($row);
    }

    // Extract timestamps. Nodes are already in the static entity cache by
    // this point (the row plugin loaded them during render()), so
    // node_load() calls below are cache hits, not real queries.
    $timestamps = array();
    foreach ($this->view->result as $i => $row) {
      $timestamps[$i] = $this->get_timestamp($row);
    }

    backdrop_add_js(array(
      'enhancedTimeline' => array(
        'minHeight'     => (int) $this->options['min_height'],
        'heightPerYear' => (int) $this->options['height_per_year'],
        'layout'        => $this->options['layout'],
        'spacing'       => $this->options['spacing'],
      ),
    ), 'setting');

    backdrop_add_js(backdrop_get_path('module', 'simple_timeline') . '/js/enhanced_timeline.js');
    backdrop_add_css(backdrop_get_path('module', 'simple_timeline') . '/css/enhanced_timeline.css');

    return theme('views_view_enhanced_timeline', array(
      'view'       => $this->view,
      'options'    => $this->options,
      'rows'       => $rows,
      'timestamps' => $timestamps,
    ));
  }

  /**
   * Returns a Unix timestamp for the given result row.
   *
   * When date_source is 'field', attempts to read a custom date field value.
   * Falls back to the node created date in all cases.
   *
   * @param object $row
   *   A single Views result row.
   *
   * @return int
   *   Unix timestamp, or 0 if none can be determined.
   */
  protected function get_timestamp($row) {
    if (empty($row->nid)) {
      return 0;
    }

    $node = node_load($row->nid);
    if (!$node) {
      return 0;
    }

    if ($this->options['date_source'] === 'field' && !empty($this->options['date_field'])) {
      $items = field_get_items('node', $node, $this->options['date_field']);
      if ($items && !empty($items[0]['value'])) {
        return (int) strtotime($items[0]['value']);
      }
    }

    // Default: node created date.
    return (int) $node->created;
  }

}
