<?php
/**
 * @file
 * Helper functions for the Custom Crop module.
 */

/**
 * Returns an array of supported widget types or checks if a type is supported.
 *
 * @param $widget_type
 *   If set, this function will return a boolean indicating if $widget_type
 *   is supported.
 * @param $settings
 *   Only include widgets that support these setting(s).
 *
 * @return
 *   Array of widget types.
 */
function custom_crop_supported_widgets($widget_type = NULL, $settings = array()) {
  // List settings that can differ from widget to widget.
  $optional_settings = array(
    'thumblist',
    'inline_crop',
    'instant_crop',
  );

  // List all widgets and the optional settings they support.
  $widgets = array(
    'image_image' => $optional_settings,
    'media_generic' => array('thumblist'),
  );

  // Make sure $settings contains only valid entries.
  if (!empty($settings)) {
    if (!is_array($settings)) {
      $settings = array($settings);
    }

    $settings = array_intersect($settings, $optional_settings);
  }

  if (empty($settings)) {
    // No settings required.
    $result = array_keys($widgets);
  }
  else {
    // Filter all widgets that don't support the required settings.
    $result = array();
    foreach ($widgets as $name => $widget_settings) {
      if (!count(array_diff($settings, $widget_settings))) {
        $result[] = $name;
      }
    }
  }

  if (!empty($widget_type)) {
    return in_array($widget_type, $result);
  }
  else {
    return $result;
  }
}

/**
 * Returns the default widget settings.
 *
 * @return
 *   Array of default widget settings.
 */
function custom_crop_default_widget_settings() {
  $defaults = array(
    // Enable Custom Crop?
    'custom_crop_enable' => FALSE,
    // Show a list of thubnails instead of a selection list or button?
    'custom_crop_thumblist' => FALSE,
    // Enable inline cropping?
    'custom_crop_inline_crop' => FALSE,
    // Show the crop info (width, height...)?
    'custom_crop_crop_info' => TRUE,
    // Automatically update the preview image?
    'custom_crop_instant_preview' => TRUE,
    // Open the crop tool after uploading?
    'custom_crop_instant_crop' => FALSE,
    // Show a default crop area when opening an uncropped image?
    'custom_crop_default_crop_area' => TRUE,
    // Exclude or include the selected styles?
    'custom_crop_styles_mode' => 'exclude',
    // List of selected styles.
    'custom_crop_styles_list' => array(),
    // List of required crop selections.
    'custom_crop_require_cropping' => array(),
  );

  if (module_exists('insert')) {
    // Filter all styles without a Custom Crop effect?
    $defaults['custom_crop_filter_insert'] = TRUE;
  }

  return $defaults;
}

/**
 * Get the unique javascript crop settings identifier.
 *
 * @param $data
 *   Field instance array (preferred) or file object.
 *
 * @return
 *   Unique javascript crop settings identifier.
 */
function custom_crop_js_identifier($data) {
  if (is_array($data) && !empty($data['field_name'])) {
    return drupal_clean_css_identifier($data['field_name']);
  }
  elseif (is_object($data) && isset($data->fid)) {
    return 'custom_crop-file-' . $data->fid;
  }
  else {
    return 'custom_crop-' . md5(serialize($data));
  }
}

/**
 * Gets the crop area for an image.
 *
 * @param $file
 *   Path to an image file.
 * @param $style_name
 *   Image style machine name, or empty for all styles.
 *
 * @return
 *   An object defining the cropping area with following items:
 *   - "style_name": The machine name of the image style this cropping area applies on.
 *   - "x": An integer representing the top left corner's x-position in pixels.
 *   - "y": An integer representing the top left corner's y-position in pixels.
 *   - "width": An integer representing the width in pixels.
 *   - "height": An integer representing the height in pixels.
 *   If the style machine name was empty an object with all the styles is returned.
 */
function custom_crop_load_crop_selection($file, $style_name = NULL) {
  if (empty($style_name)) {
    $result = db_query('SELECT c.style_name, c.x, c.y, c.width, c.height FROM {custom_crop} c INNER JOIN {file_managed} f ON c.fid = f.fid WHERE f.uri = :uri', array(
      ':uri' => $file,
    ));

    return $result->fetchAll();
  }
  else {
    $result = db_query('SELECT c.x, c.y, c.width, c.height FROM {custom_crop} c INNER JOIN {file_managed} f ON c.fid = f.fid WHERE c.style_name = :name AND f.uri = :uri', array(
      ':name' => $style_name,
      ':uri' => $file,
    ));

    return $result->fetchObject();
  }

  return NULL;
}

/**
 * Returns the styles that have crop settings.
 *
 * @param $include_reuse
 *   Set to TRUE to include styles with a Custom Crop reuse effect.
 *
 * @return
 *   An array of styles with the style name as key and the effect data as value.
 */
function custom_crop_styles_with_crop($include_reuse = FALSE) {
  $hascrop = &drupal_static(__FUNCTION__);

  if (!is_array($hascrop)) {
    $hascrop = array(array(), array());

    foreach (image_styles() as $style_name => $style) {
      if (!empty($style['effects'])) {
        // Check if the first effect is a Custom Crop cropping effect.
        $effect = reset($style['effects']);

        if (_custom_crop_is_own_effect($effect)) {
          $hascrop[1][$style_name] = $effect;

          if (_custom_crop_is_own_effect($effect, TRUE)) {
            $hascrop[0][$style_name] = $effect;
          }
        }
      }
    }
  }

  return $hascrop[(int) $include_reuse];
}

/**
 * Get the list of required image styles from the widget settings.
 *
 * @param $settings
 *   Widget settings array.
 */
function custom_crop_instance_required_styles($settings) {
  // Array of required styles.
  $required = array_values($settings['custom_crop_require_cropping']);

  // Make sure to exclude unavailable styles.
  if (!empty($required) && !empty($settings['custom_crop_styles_list'])) {
    if ($settings['custom_crop_styles_mode'] == 'include') {
      $required = array_intersect($required, array_values($settings['custom_crop_styles_list']));
    }
    else {
      $required = array_diff($required, array_values($settings['custom_crop_styles_list']));
    }
  }

  return $required;
}

/**
 * Update or remove a style name in all Custom Crop field widgets.
 *
 * @param $style_name
 *   Current image style name.
 * @param $new_style_name
 *   New image style name if renamed, a NULL value will remove the style from the settings.
 */
function _custom_crop_field_widget_update_names_in_settings($style_name, $new_style_name = NULL) {
  foreach (field_info_fields() as $field) {
    if ($field['module'] == 'image') {
      foreach ($field['bundles'] as $entity_type => $bundles) {
        foreach ($bundles as $bundle) {
          // Check each instance for processing.
          $instance = field_info_instance($entity_type, $field['field_name'], $bundle);
          $settings = &$instance['widget']['settings'];

          if (custom_crop_supported_widgets($instance['widget']['type']) && (!empty($settings['custom_crop_require_cropping']) || !empty($settings['custom_crop_styles_list']))) {
            $list = array();

            // Add all existing settings to the list.
            if (!empty($settings['custom_crop_require_cropping'])) {
              $list['custom_crop_require_cropping'] = &$settings['custom_crop_require_cropping'];
            }

            if (!empty($settings['custom_crop_styles_list'])) {
              $list['custom_crop_styles_list'] = &$settings['custom_crop_styles_list'];
            }

            // Process all settings.
            foreach ($list as $key => &$item) {
              if (isset($item[$style_name])) {
                unset($item[$style_name]);

                if (!is_null($new_style_name)) {
                  $item[$new_style_name] = $new_style_name;
                }
                elseif (empty($require)) {
                  unset($settings[$key]);
                }
              }
              else {
                // Not processed, so remove it from the list.
                unset($list[$key]);
              }
            }

            if (!empty($list)) {
              // Settings where updated, save the instance.
              field_update_instance($instance);
            }
          }
        }
      }
    }
  }
}

/**
 * Update or remove a style name in all Custom Crop reuse image effects.
 *
 * @param $style_name
 *   Current image style name.
 * @param $new_style_name
 *   New image style name if renamed, a NULL value will remove the effect from the style.
 */
function _custom_crop_reuse_effect_update_names_in_settings($style_name, $new_style_name = NULL) {
  foreach (image_styles() as $style) {
    if (!empty($style['effects'])) {
      // Check if the first effect is a Custom Crop effect.
      $effect = reset($style['effects']);

      // Check if this is a Custom Crop reuse effect that needs an update.
      if (_custom_crop_is_own_effect($effect, FALSE) && $effect['data']['reusestyle'] == $style_name) {
        if (is_null($new_style_name)) {
          image_effect_delete($effect);
        }
        else {
          $effect['data']['reusestyle'] = $new_style_name;
          image_effect_save($effect);
        }
      }
    }
  }
}

/**
 * Transform a style name into a more readable variant.
 *
 * @param $style_name
 *   Image style name.
 *
 * @return
 *   Cleaned-up image style name.
 */
function _custom_crop_image_style_name($style_name) {
  global $language;
  static $custom_strings;

  $langcode = (isset($language->language) ? $language->language : 'en');

  // Load custom string for overriding.
  if (!isset($custom_strings[$langcode])) {
    $custom_strings[$langcode] = variable_get('locale_custom_strings_' . $langcode, array());
  }

  // Get the human readable name from the custom strings or make it ourself.
  if (isset($custom_strings[$langcode]['']['image-style-' . $style_name])) {
    return $custom_strings[$langcode]['']['image-style-' . $style_name];
  }
  else {
    return ucwords(str_replace('_', ' ', $style_name));
  }
}

/**
 * Checks if the effect is a Custom Crop effect.
 *
 * @param $effect
 *   Image style effect information array.
 * @param $crop_effect
 *   Set to TRUE to require a cropping effect or set to FALSE to require the
 *   reuse effect.
 *
 * @return
 *   TRUE if this is a Custom Crop (cropping/reuse) effect, FALSE otherwise.
 */
function _custom_crop_is_own_effect($effect, $crop_effect = NULL) {
  return ($effect['module'] == 'custom_crop' && (is_null($crop_effect) || ($crop_effect ^ ($effect['name'] == 'custom_crop_reuse'))));
}



/**
 * Form Builder; Configuration settings for the crop and scale effect.
 *
 * @param $data
 *   The current configuration for this crop and scale effect.
 *
 * @return
 *   The form structure array.
 */
function custom_crop_crop_and_scale_form($data) {
  $form = image_scale_form($data);

  $form['width']['#required'] = TRUE;
  $form['height']['#required'] = TRUE;

  $form['respectminimum'] = array(
    '#type' => 'checkbox',
    '#title' => t('Respect minimum'),
    '#description' => t("Make sure the selected crop area is at least as big as the destination size. This doesn't enforce minimum image dimensions."),
    '#default_value' => (isset($data['respectminimum']) ? $data['respectminimum'] : TRUE),
  );

  $form['onlyscaleifcrop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only scale if cropped'),
    '#description' => t('Only scale the image if it was manually cropped.'),
    '#default_value' => (isset($data['onlyscaleifcrop']) ? $data['onlyscaleifcrop'] : FALSE),
  );

  return $form;
}

/**
 * Form Builder; Configuration settings for crop effect.
 *
 * @param $data
 *   The current configuration for this crop effect.
 *
 * @return
 *   The form structure array.
 */
function custom_crop_crop_form($data) {
  $form = image_resize_form($data);

  $form['width']['#title'] = t('Minimum crop width');
  $form['width']['#description'] = t('Only applied if a user tries to crop, this enforces no minimum image width!');
  $form['width']['#required'] = FALSE;

  $form['height']['#title'] = t('Minimum crop height');
  $form['height']['#description'] = t('Only applied if a user tries to crop, this enforces no minimum image height!');
  $form['height']['#required'] = FALSE;

  $form['keepproportions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Maintain proportions'),
    '#description' => t('Maintain the proportions while cropping. This requires seting a width and height.'),
    '#default_value' => (isset($data['keepproportions']) ? $data['keepproportions'] : FALSE),
    '#element_validate' => array('custom_crop_keepproportions_validate'),
    '#states' => array(
      'disabled' => array(
        'input[name="data[width]"]' => array('empty' => TRUE),
        'input[name="data[height]"]' => array('empty' => TRUE),
      ),
      'unchecked' => array(
        'input[name="data[width]"]' => array('empty' => TRUE),
        'input[name="data[height]"]' => array('empty' => TRUE),
      ),
    ),
  );

  return $form;
}

/**
 * Element validation handler; validates the keepproportions checkbox from the
 * crop effect form.
 */
function custom_crop_keepproportions_validate($element, $form_state) {
  $values = $form_state['values']['data'];

  if ($values['keepproportions'] && (!$values['width'] || !$values['height'])) {
    form_set_error('data][keepproportions', t('Set a width and height if proportions should be maintained.'));
  }
}


/**
 * Form Builder; Configuration settings for the reuse effect.
 *
 * @param $data
 *   The current configuration for this reuse effect.
 *
 * @return
 *   The form structure array.
 */
function custom_crop_reuse_form($data) {
  // Load all image styles that have a cropping effect and exclude the style
  // that's currently being edited.
  $styles = custom_crop_styles_with_crop();
  $current = arg(5);
  if (isset($styles[$current])) {
    unset($styles[$current]);
  }

  if (!count($styles)) {
    drupal_set_message(t('No Custom Crop enabled image style could be found. To reuse a crop selection, you need to create at least one image style that uses Custom Crop.'), 'warning');
    drupal_goto('admin/config/media/image-styles/edit/' . $current);
  }

  $form['reusestyle'] = array(
    '#type' => 'select',
    '#title' => t('Base image style'),
    '#description' => t('Base this image style upon the crop selection of the selected style.'),
    '#options' => drupal_map_assoc(array_keys($styles), '_custom_crop_image_style_name'),
    '#default_value' => (isset($data['reusestyle']) ? $data['reusestyle'] : ''),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function custom_crop_form_image_effect_form_alter(&$form, &$form_state) {
  if (!is_array($form['#submit'])) {
    $form['#submit'] = array('image_effect_form_submit');
  }

  // If it's a Custom Crop effect do a custom submit handler first.
  // Otherwise reorder the effects after the submit.
  if (_custom_crop_is_own_effect($form_state['image_effect'])) {
    array_unshift($form['#submit'], 'custom_crop_image_effect_form_submit');
  }
  elseif (!empty($form_state['image_style']['effects'])) {
    $form['#submit'][] = 'custom_crop_force_effect_order';
  }
}

/**
 * Effect form submit handler; Sets the image style name and weight of the effect.
 *
 * @param $form
 *   Effect form array.
 * @param $form_state
 *   Form state array.
 */
function custom_crop_image_effect_form_submit($form, &$form_state) {
  $style = $form_state['image_style'];
  $effect = $form_state['image_effect'];

  // Set style name so the effect can use it.
  $form_state['values']['data']['style_name'] = $style['name'];

  if (count($style['effects']) == 0) {
    // First effect, so weight is 0.
    $form_state['values']['weight'] = 0;
  }
  elseif (!isset($effect['ieid'])) {
    // New effect, the weight must be the weight of the first effect minus one.
    $first = reset($style['effects']);
    $form_state['values']['weight'] = $first["weight"] - 1;
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function custom_crop_form_image_effect_delete_form_alter(&$form, &$form_state) {
  if (!is_array($form['#submit'])) {
    $form['#submit'] = array('image_effect_delete_form_submit');
  }

  if (_custom_crop_is_own_effect($form_state['image_effect'], TRUE)) {
    // Submit handler to cleanup the crop selections.
    array_unshift($form['#submit'], 'custom_crop_image_effect_delete_form_submit');
  }
}

/**
 * Effect deletion form submit handler; Cleanup old selections.
 *
 * @param $form
 *   Effect form array.
 * @param $form_state
 *   Form state array.
 */
function custom_crop_image_effect_delete_form_submit($form, &$form_state) {
  custom_crop_image_style_delete($form_state['image_style']);
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function custom_crop_form_image_style_form_alter(&$form, &$form_state) {
  $style = $form_state['image_style'];

  if (!empty($style['effects'])) {
    if (!is_array($form['#submit'])) {
      $form['#submit'] = array('image_style_form_submit');
    }

    // Reorder effects on submit.
    $form['#submit'][] = 'custom_crop_force_effect_order';

    // Update style name in effect data.
    $form['#submit'][] = 'custom_crop_update_style_name';

    // Filter all Custom Crop effects from the form if there's already a Custom Crop effect.
    if (isset($form['effects']['new']['new'])) {
      foreach ($style['effects'] as $effect) {
        if (_custom_crop_is_own_effect($effect)) {
          $form['effects']['new']['new']['#options'] = array_diff_key($form['effects']['new']['new']['#options'], custom_crop_image_effect_info());
          break;
        }
      }
    }
  }
}

/**
 * Effect and style form submit handler; Force the Custom Crop effect to be the first one.
 *
 * @param $form
 *   Effect or style form array.
 * @param $form_state
 *   Form state array.
 */
function custom_crop_force_effect_order($form, &$form_state) {
  // In both forms (style and effect) we find the style in the same key.
  $style = $form_state['image_style'];

  // If it's the style form, check if the style name was updated.
  if ($form['#form_id'] == 'image_style_form' && isset($form_state['values']['name']) && $style['name'] != $form_state['values']['name']) {
    $style['name'] = $form_state['values']['name'];
  }

  // The style array is received trough the loader to make sure the function
  // works with the latest version.
  $style = image_style_load($style['name']);

  if (!empty($style['effects'])) {
    foreach ($style['effects'] as $eid => $effect) {
      if (_custom_crop_is_own_effect($effect)) {
        $first = reset($style['effects']);

        if ($eid != key($style['effects'])) {
          // Change the weight of this effect to the weight of the first effect minus 1.
          $effect['weight'] = $first['weight'] - 1;
          image_effect_save($effect);
        }

        break;
      }
    }
  }
}

/**
 * Style form submit handler; Update the style name in the effect data and in the widgets.
 *
 * @param $form
 *   Style form array.
 * @param $form_state
 *   Form state array.
 */
function custom_crop_update_style_name($form, &$form_state) {
  $style = $form_state['image_style'];

  // Check if the style name should be updated.
  if (!empty($style['effects']) && isset($form_state['values']['name']) && $style['name'] != $form_state['values']['name']) {
    $effect = reset($style['effects']);

    if (_custom_crop_is_own_effect($effect, TRUE)) {
      // Change the style name in the effect data and save it.
      $effect['data']['style_name'] = $form_state['values']['name'];
      image_effect_save($effect);

      // Update all widgets and reuse effects.
      _custom_crop_field_widget_update_names_in_settings($style['name'], $form_state['values']['name']);
      _custom_crop_reuse_effect_update_names_in_settings($style['name'], $form_state['values']['name']);

      // Update all saved crop selections.
      db_update('custom_crop')
        ->fields(array('style_name' => $form_state['values']['name']))
        ->condition('style_name', $style['name'])
        ->execute();
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function custom_crop_form_field_ui_field_edit_form_alter(&$form, $form_state) {
  // Get the instance and widget type.
  $element = &$form['instance'];
  $widget_type = $element['widget']['type']['#value'];

  if (custom_crop_supported_widgets($widget_type)) {
    // Get the field instance and setting.
    $instance = field_info_instance($element['entity_type']['#value'], $element['field_name']['#value'], $element['bundle']['#value']);
    $settings = $instance['widget']['settings'];

    // Add our custom field settings.
    $element['settings'] += custom_crop_field_widget_settings_form($widget_type, $settings);

    // Alter the existing form elements.
    custom_crop_field_widget_settings_form_alter($element, $settings);
  }
}

/**
 * Add the Custom Crop field widget settings.
 *
 * @param $widget_type
 *   Widget type.
 * @param $settings
 *   Current settings.
 *
 * @return
 *   Form elements to add.
 */
function custom_crop_field_widget_settings_form($widget_type, $settings) {
  $form = array();

  // Build the styles list array.
  $style_options = array_keys(custom_crop_styles_with_crop());
  $style_options = drupal_map_assoc($style_options, '_custom_crop_image_style_name');

  // Add the Custom Crop fieldset.
  $form['custom_crop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Crop'),
    '#description' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 25,
    '#parents' => array('instance', 'widget', 'settings'),
  );

  $form['custom_crop']['custom_crop_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#description' => t('Enable the Custom Crop features.'),
    '#default_value' => ($settings['custom_crop_enable'] && !empty($style_options)),
    '#disabled' => empty($style_options),
  );

  if (custom_crop_supported_widgets($widget_type, 'thumblist')) {
    $form['custom_crop']['custom_crop_thumblist'] = array(
      '#type' => 'checkbox',
      '#title' => t('List thumbs'),
      '#description' => t('Instead showing a button or selection list, show all thumbails (this will disable the preview thumbnail).'),
      '#default_value' => $settings['custom_crop_thumblist'],
    );
  }

  if (custom_crop_supported_widgets($widget_type, 'inline_crop')) {
    $form['custom_crop']['custom_crop_inline_crop'] = array(
      '#type' => 'checkbox',
      '#title' => t('Inline cropping'),
      '#description' => t('Instead of opening an overlay, use inline cropping.'),
      '#default_value' => $settings['custom_crop_inline_crop'],
    );
  }

  $form['custom_crop']['custom_crop_crop_info'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show crop info'),
    '#description' => t('Show the crop selection details.'),
    '#default_value' => $settings['custom_crop_crop_info'],
  );

  $form['custom_crop']['custom_crop_instant_preview'] = array(
    '#type' => 'checkbox',
    '#title' => t('Instant preview'),
    '#description' => t('Show an instant preview of the crop selection.'),
    '#default_value' => $settings['custom_crop_instant_preview'],
  );

  if (custom_crop_supported_widgets($widget_type, 'instant_crop')) {
    $form['custom_crop']['custom_crop_instant_crop'] = array(
      '#type' => 'checkbox',
      '#title' => t('Crop after upload'),
      '#description' => t('Open the cropping tool direct after the file upload. Note that this will only work if you <strong>enable only one image style</strong>.'),
      '#default_value' => $settings['custom_crop_instant_crop'],
    );
  }

  $form['custom_crop']['custom_crop_default_crop_area'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default crop area'),
    '#description' => t('Create a default crop area when opening the croptool for uncropped images.'),
    '#default_value' => $settings['custom_crop_default_crop_area'],
  );

  $form['custom_crop']['custom_crop_styles_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Styles list mode'),
    '#options' => array(
      'exclude' => t('Exclude selected styles'),
      'include' => t('Include selected styles'),
    ),
    '#default_value' => $settings['custom_crop_styles_mode'],
  );

  $form['custom_crop']['custom_crop_styles_list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Styles list'),
    '#description' => t('Select all styles that should be shown or hidden (as selected above) in the widget. Please note that hiding styles will override requiring them and existing cropping selections will be kept.'),
    '#options' => $style_options,
    '#default_value' => $settings['custom_crop_styles_list'],
    '#multicolumn' => array('width' => 3),
  );

  $form['custom_crop']['custom_crop_require_cropping'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Required croppings'),
    '#description' => t('Select all styles that must have a crop selection.'),
    '#options' => $style_options,
    '#default_value' => $settings['custom_crop_require_cropping'],
    '#multicolumn' => array('width' => 3),
  );

  return $form;
}

/**
 * Alters the existing field widget settings form elements.
 *
 * @param $form
 *   Instance form elements array.
 */
function custom_crop_field_widget_settings_form_alter(&$form, $settings) {
  if ($form['widget']['type']['#value'] == 'image_image' && isset($settings['preview_image_style'])) {
    // Disable the image field preview if the thumblist option is enabled.
    $form['widget']['settings']['preview_image_style']['#states'] = array(
      'disabled' => array(
        'input[name="instance[widget][settings][custom_crop_enable]"]' => array('checked' => TRUE),
        'input[name="instance[widget][settings][custom_crop_thumblist]"]' => array('checked' => TRUE),
      ),
    );
  }

  if (module_exists('insert')) {
    // Get the reference to the insert fieldset.
    $insert = &$form['settings']['insert'];

    // Make sure the insert checkbox stays on top.
    $insert['insert']['#weight'] = -2;

    // Add an option to exclude image styles without a Custom Crop effect.
    $insert['custom_crop_filter_insert'] = array(
      '#type' => 'checkbox',
      '#title' => t('Only Custom Crop styles'),
      '#description' => t('Enable this option to filter out all image styles without a Custom Crop effect.'),
      '#default_value' => !empty($settings['custom_crop_filter_insert']),
      '#weight' => -1,
      '#states' => array(
        'disabled' => array(
          'input[name="instance[widget][settings][custom_crop_enable]"]' => array('checked' => FALSE),
        ),
      )
    );

    // If checked, remove all non-Custom Crop styles.
    if (!empty($settings['custom_crop_enable']) && !empty($settings['custom_crop_filter_insert'])) {
      $styles = array_merge(array('image' => 0), custom_crop_insert_styles());

      $insert['insert_styles']['#options'] = array_intersect_key($insert['insert_styles']['#options'], $styles);
      $insert['insert_default']['#options'] = array_intersect_key($insert['insert_default']['#options'], $styles);
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function custom_crop_form_system_performance_settings_alter(&$form, &$form_state) {
  $form['custom_crop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Crop'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );

  $form['custom_crop']['cache_control'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reload cache-control'),
    '#description' => t('When enabled, a cache-controlling variable will be added to the cropped image path. This variable will change as soon as you re-crop the image, this way the visitor his/her browser will reload the image.'),
    '#default_value' => variable_get('custom_crop_cache_control', TRUE),
  );

  $form['#submit'][] = 'custom_crop_form_system_performance_settings_submit';

  return $form;
}

/**
 * Performance form submit handler; Update the cache-control settings.
 *
 * @param $form
 *   Effect form array.
 * @param $form_state
 *   Form state array.
 */
function custom_crop_form_system_performance_settings_submit($form, &$form_state) {
  $cache_control = (bool) $form_state['values']['custom_crop']['cache_control'];

  variable_set('custom_crop_cache_control', $cache_control);

  if (!$cache_control) {
    // Clear the cached image paths.
    cache_clear_all('custom_crop', 'cache', TRUE);
  }
}