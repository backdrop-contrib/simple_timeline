<?php

/**
 * @file
 * Contains \Drupal\simple_timeline\Plugin\views\row\SimpleTimelineRow.
 */

namespace Drupal\simple_timeline\Plugin\views\row;

use Drupal\Core\Form\FormStateInterface;
use Drupal\views\Plugin\views\row\RowPluginBase;

/**
 * Renders an SimpleTimeline item based on fields.
 *
 * @ViewsRow(
 *   id = "simple_timeline_row",
 *   title = @Translation("Simple Timeline Item"),
 *   help = @Translation("Renderer for a timeline item."),
 *   theme = "simple_timeline_fields",
 *   display_types = {"normal"}
 * )
 */
class SimpleTimelineRow extends RowPluginBase {

  /**
   * Does the row plugin support to add fields to it's output.
   *
   * @var bool
   */
  protected $usesFields = TRUE;

  /**
   * {@inheritdoc}
   */
  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['simple_timeline_date'] = array('default' => '');
    $options['simple_timeline_date_separator'] = array('default' => '');
    $options['simple_timeline_text'] = array('default' => '');
    $options['simple_timeline_text_separator'] = array('default' => '');
    $options['simple_timeline_image'] = array('default' => '');
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    parent::buildOptionsForm($form, $form_state);

    $view_fields_labels = $this->displayHandler->getFieldLabels();

    $form['simple_timeline_date'] = array(
      '#type' => 'checkboxes',
      '#title' => $this->t('Timeline Date fields'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['simple_timeline_date'],
      '#description' => $this->t('Selected fields will be displayed next to each other in the Date section.'),
    );

    $form['simple_timeline_date_separator'] = array(
      '#title' => $this->t('Date Separator'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $this->options['simple_timeline_date_separator'],
      '#description' => $this->t('The separator may be placed between inline fields to keep them from squishing up next to each other. You can use HTML in this field.'),
    );

    $form['simple_timeline_text'] = array(
      '#type' => 'checkboxes',
      '#title' => $this->t('Timeline Text fields'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['simple_timeline_text'],
      '#description' => $this->t('Selected fields will be displayed next to each other in the Text section'),
    );

    $form['simple_timeline_text_separator'] = array(
      '#title' => $this->t('Text Separator'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $this->options['simple_timeline_text_separator'],
      '#description' => $this->t('The separator may be placed between inline fields to keep them from squishing up next to each other. You can use HTML in this field.'),
    );

    $form['simple_timeline_image'] = array(
      '#type' => 'radios',
      '#title' => $this->t('Timeline Image field'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['simple_timeline_image'],
      '#description' => $this->t('The image to be displayed with each item'),
    );

  }

  /**
   * {@inheritdoc}
   */
  public function render($row) {
    return array(
      '#theme' => $this->themeFunctions(),
      '#view' => $this->view,
      '#options' => $this->options,
      '#row' => $row,
    );
  }

}
