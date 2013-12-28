<?php
/**
 * Created by JetBrains PhpStorm.
 * User: alan
 */

/**
 * @file
 * Contains the simple timeline style plugin.
 */

/**
 * Style plugin to render each item on a simple timeline.
 *
 * @ingroup views_style_plugins
 */
class simple_timeline_style_plugin extends views_plugin_style_list
{
  function option_definition() {
    $options = parent::option_definition();
    $options['wrapper_class'] = array('default' => 'item-list simple_timeline');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type']['#type'] = 'hidden';
    $form['wrapper_class']['#type'] = 'hidden';
  }

  public function render() {
    drupal_add_css(drupal_get_path('module', 'simple_timeline') . '/simple_timeline.css', 'file');
    return parent::render();
  }

  public function validate() {
    $errors = array();

    $row_plugin = get_class($this->row_plugin);
    if (!in_array($row_plugin, array(
        'views_plugin_row_node_view',
        'panels_views_plugin_row_fields',
        'simple_timeline_row_plugin',
    ))
    ) {
      $errors[] = t('You have not selected an appropriate row plugin. your timeline will break');
    }
    return $errors;
  }
}
