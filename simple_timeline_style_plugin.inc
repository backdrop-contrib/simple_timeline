<?php
/**
 * @file
 * Contains the simple timeline style plugin.
 * Created by JetBrains PhpStorm.
 * User: alan
 */

/**
 * Style plugin to render each item on a simple timeline.
 *
 * @ingroup views_style_plugins
 */
class simple_timeline_style_plugin extends views_plugin_style_list {

  /**
   * Modifies the options form inherited by this plugin.
   *
   * @param array $form
   *   The form being generated.
   * @param array $form_state
   *   The state that the form has been posted in.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type']['#type'] = 'hidden';

    $form['row_class_special']['#title'] .= t(' (required on timelines)');
    $form['row_class_special']['#disabled'] = TRUE;
  }

  /**
   * Renders the data provided to this plugin.
   *
   * @return string
   *   The rendered html.
   */
  public function render() {
    // @FIXME
// CSS, JavaScript, and libraries are now attached directly to render arrays
// using the #attached property. drupal_add_css() is internal and should never be
// called directly.
// 
// 
// @see https://www.drupal.org/node/2169605
// drupal_add_css(drupal_get_path('module', 'simple_timeline') . '/simple_timeline.css', 'file');


    // Set needed values for basic timeline proper functioning.
    $this->options['wrapper_class'] .= ' simple-timeline';

    return parent::render();
  }

  /**
   * Validate whether this plugin is configured correctly.
   *
   * @return array
   *   The list of errors to be displayed.
   */
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
