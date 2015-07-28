var custom_crop = {croptool: null, oldSelection: null, widget: null, output: null};

(function ($) {

/**
 * Mark required image styles and trigger the onchange event of all (hidden) fields that store
 * crop data. This way all css classes for the crop lists/buttons will be updated and the default
 * image preview will be changed to the cropped image.
 */
custom_crop.init = function() {
  var fields = Drupal.settings.custom_crop.fields;

  for (var identifier in fields) {
    for (var k in fields[identifier].required) {
      $('.custom_crop-identifier-' + identifier + ' option[value="' + fields[identifier].required[k] + '"]')
        .addClass('custom_crop-style-required');
    }
  }

  $('.custom_crop-cropdata').trigger('change');
}

/**
 * Callback triggerd after an image upload.
 *
 * @param context
 *    Container of the uploaded image widget.
 */
custom_crop.afterUpload = function(context) {
  custom_crop.init();

  var fields = Drupal.settings.custom_crop.fields;

  for (var identifier in fields) {
    if (fields[identifier].instantCrop) {
      if ($('.custom_crop-cropdata', context).length == 1) {
        $('.custom_crop-style-button, .custom_crop-style-thumb', context).trigger('mousedown');
      }
    }
  }
}

/**
 * Open the cropping tool for an image.
 *
 * @param identifier
 *    Unique crop settings identifier.
 * @param style
 *   The image style name or selection list triggering this event.
 * @param fid
 *   The file id of the image the user is about to crop.
 */
custom_crop.showCroptool = function(identifier, style, fid) {
  var styleName, styleSelect, cropType, origContainer, conWidth, conHeight;

  if (custom_crop.croptool) {
    // Close the current croptool.
    custom_crop.closeCroptool();
  }

  // Get the style name.
  if (typeof style == 'string') {
    styleName = style
  }
  else {
    styleSelect = $(style);
    styleName = styleSelect.val();
  }

  $('.custom_crop-file-' + fid + '-holder img').imagesLoaded(function() {
    // IE executes this callback twice, so we check if the custom_crop.croptool
    // has already been set and skip the rest if this is the case.
    if (!custom_crop.croptool) {
      // Determine the croptool type.
      if ($('#custom_crop-overlay-' + fid).length == 1) {
        cropType = 'overlay';
        origContainer = $('#custom_crop-overlay-' + fid);
      }
      else {
        cropType = 'inline';
        origContainer = $('#custom_crop-inline-' + fid);
      }

      // Get the crop settings.
      var settings = Drupal.settings.custom_crop.styles[styleName] || {};

      // Get the destination field and the current selection.
      custom_crop.output = $('#custom_crop-area-' + fid + '-' + styleName);
      custom_crop.oldSelection = custom_crop.parseStringSelection(custom_crop.output.val());

      // Create the croptool.
      custom_crop.croptool = origContainer.clone()
        .removeAttr('id')
        .removeClass('element-hidden');

      // Get the container maximum width and height.
      if (cropType == 'overlay') {
        conWidth = $(window).width();
        conHeight = $(window).height();
      }
      else {
        conWidth = origContainer.parent().innerWidth();
        conHeight = $(window).height();
      }

      // Tool width and height.
      custom_crop.croptool.css('width', conWidth + 'px');

      if (cropType == 'overlay') {
        custom_crop.croptool.css('height', conHeight + 'px');
      }

      // Get the image and its dimensions.
      var image = $('.custom_crop-image', origContainer);
      var width = custom_crop.parseInt(image.width()) || custom_crop.parseInt(image.attr('width'));
      var height = custom_crop.parseInt(image.height()) || custom_crop.parseInt(image.attr('height'));

      // Scale the image to fit the maximum width and height (so all is visible).
      var maxWidth = conWidth - custom_crop.parseInt(image.css('marginLeft')) - custom_crop.parseInt(image.css('marginRight'));
      var maxHeight = conHeight - custom_crop.parseInt(image.css('marginTop')) - custom_crop.parseInt(image.css('marginBottom'));

      // Calculate the clone image dimensions.
      var resized = custom_crop.resizeDimensions(width, height, maxWidth, maxHeight);

      // Set the new width and height to the cloned image.
      image = $('.custom_crop-image', custom_crop.croptool)
        .css('width', resized.width + 'px')
        .css('height', resized.height + 'px');

      // Basic imgAreaSelect options; All options are set - also if it's their
      // default value - because IE doesn't seem to use the default values.
      var options = {
        handles: true,
        instance: true,
        keys: true,
        movable: true,
        resizable: true,
        parent: image.parent(),
        imageWidth: width,
        imageHeight: height,
        onSelectChange: custom_crop.updateSelection
      };

      // Additional options based upon the effect.
      if (settings) {
        switch (settings.effect) {
          // Crop and scale effect.
          case 'custom_crop_crop_and_scale':
            options.aspectRatio = settings.data.width + ':' + settings.data.height;

            if (settings.data.respectminimum) {
              // Crop at least the minimum.
              options.minWidth = custom_crop.parseInt(settings.data.width);
              options.minHeight = custom_crop.parseInt(settings.data.height);
            }
            break;

          // Crop effect.
          case 'custom_crop_crop':
            if (settings.data.width) {
              options.minWidth = custom_crop.parseInt(settings.data.width);
            }

            if (settings.data.height) {
              options.minHeight = custom_crop.parseInt(settings.data.height);
            }

            if (typeof settings.data.keepproportions != 'undefined' && settings.data.keepproportions) {
              options.aspectRatio = settings.data.width + ':' + settings.data.height;
            }
        }
      }

      // Set the image style name.
      $('.custom_crop-image-style', custom_crop.croptool).text(styleName);

      if (typeof styleSelect != 'undefined') {
        // Reset the image style selection list.
        styleSelect.val('');
        styleSelect.blur();
      }
      else {
        // Hide the crop button.
        $('.custom_crop-style-button-' + fid).hide();
      }

      // Append the cropping area (last, to prevent that '_11' is undefined).
      if (cropType == 'overlay') {
        $('body').append(custom_crop.croptool);
      }
      else {
        origContainer.parent().append(custom_crop.croptool);
      }

      // Create the crop widget.
      custom_crop.widget = image.imgAreaSelect(options);

      // IE seems to have some issues with the imgAreaSelect $parent variable,
      // so we set the options again to initialize it correctly.
      if ($.browser && $.browser.msie) {
        custom_crop.widget.setOptions(options);
      }

      // Insert the instant preview image.
      var instantPreview = $('.custom_crop-instantpreview', custom_crop.croptool);
      if (instantPreview.length) {
        // Save the current width as maximum width and height.
        instantPreview
          .data('maxWidth', instantPreview.width())
          .data('maxHeight', instantPreview.width())
          .height(instantPreview.width());

        // Calculate the instant preview dimensions.
        resized = custom_crop.resizeDimensions(width, height, instantPreview.width(), instantPreview.width());

        // Set those dimensions.
        image.clone().appendTo(instantPreview)
          .removeClass()
          .css('width', resized.width + 'px')
          .css('height', resized.height + 'px');
      }

      if (!custom_crop.oldSelection) {
        var fields = Drupal.settings.custom_crop.fields;

        // Create a default crop area.
        if (typeof fields[identifier] == 'object' && fields[identifier].defaultCropArea) {
          var minWidth = (typeof options.minWidth != 'undefined' ? options.minWidth : 0);
          var minHeight = (typeof options.minHeight != 'undefined' ? options.minHeight : 0)

          // Set a width and height.
          custom_crop.oldSelection = {
            width: (minWidth ? minWidth * 100 : (width / 2)),
            height: (minHeight ? minHeight * 100 : (height / 2)),
            maxWidth: (width / 2),
            maxHeight: (height / 2)
          };

          // Resize the selection.
          custom_crop.oldSelection = custom_crop.resizeDimensions(custom_crop.oldSelection);

          // Make sure we respect the minimum dimensions.
          if (minWidth || minHeight) {
            if (minWidth && custom_crop.oldSelection.width < minWidth) {
              custom_crop.oldSelection.width = minWidth;

              if (minHeight) {
                custom_crop.oldSelection.height = minHeight;
              }
            }
            else if (minHeight && custom_crop.oldSelection.height < minHeight) {
              custom_crop.oldSelection.height = minHeight;

              if (minWidth) {
                custom_crop.oldSelection.width = minWidth;
              }
            }
          }

          // Center the selection.
          custom_crop.oldSelection.x1 = (width - custom_crop.oldSelection.width) / 2;
          custom_crop.oldSelection.y1 = (height - custom_crop.oldSelection.height) / 2;
          custom_crop.oldSelection.x2 = custom_crop.oldSelection.x1 + custom_crop.oldSelection.width;
          custom_crop.oldSelection.y2 = custom_crop.oldSelection.y1 + custom_crop.oldSelection.height;
        }
      }

      // Set the initial selection.
      if (custom_crop.oldSelection) {
        custom_crop.croptool.imagesLoaded(custom_crop.resetSelection);
      }

      // Handle keyboard shortcuts.
      $(document).keyup(custom_crop.handleKeyboard);
    }
  });
}

/**
 * Close the cropping tool.
 *
 * @param reset
 *   Set to true to reset the selection before closing.
 */
custom_crop.closeCroptool = function(reset) {
  if (custom_crop.croptool) {
    if (reset) {
      custom_crop.resetSelection();
    }

    custom_crop.output.trigger('change');

    custom_crop.widget.setOptions({remove: true});
    custom_crop.croptool.remove();
    custom_crop.croptool = null;
    custom_crop.oldSelection = null;
    custom_crop.widget = null;
    custom_crop.output = null;

    $('.custom_crop-style-button').show();

    $(document).unbind('keyup', custom_crop.handleKeyboard);
  }
}

/**
 * Reset the selection to the previous state.
 */
custom_crop.resetSelection = function() {
  if (custom_crop.croptool) {
    if (custom_crop.oldSelection) {
      custom_crop.widget.setSelection(custom_crop.oldSelection.x1, custom_crop.oldSelection.y1, custom_crop.oldSelection.x2, custom_crop.oldSelection.y2);
      custom_crop.widget.setOptions({hide: false, show: true});
      custom_crop.widget.update();
      custom_crop.updateSelection(null, custom_crop.oldSelection);

      // Hide reset button.
      $('.custom_crop-reset', custom_crop.croptool).hide();
    }
    else {
      custom_crop.clearSelection();
    }
  }
}

/**
 * Maximize the selection to fill the container as much as possible/allowed.
 */
custom_crop.maximizeSelection = function() {
  if (custom_crop.croptool) {
    var image = $('img.custom_crop-image', custom_crop.croptool);

    // Get the original width and height.
    var origWidth = custom_crop.parseInt(image.get(0).getAttribute('width'));
    var origHeight = custom_crop.parseInt(image.get(0).getAttribute('height'))
    var options = custom_crop.widget.getOptions();

    // Check if the ratio should be respected.
    if (typeof options.aspectRatio != 'undefined' && options.aspectRatio != '') {
      // Get the ratio.
      var ratio = options.aspectRatio.match(/([0-9]+):([0-9]+)/);
      var ratioWidth = custom_crop.parseInt(ratio[1]);
      var ratioHeight = custom_crop.parseInt(ratio[2]);

      // Crop area defaults.
      var width = origWidth;
      var height = origHeight;
      var x = 0;
      var y = 0;

      if ((ratioWidth / ratioHeight) > (origWidth / origHeight)) {
        // Crop from top and bottom.
        height = Math.floor((width / ratioWidth) * ratioHeight);
        y = Math.floor((origHeight - height) / 2);
      }
      else {
        // Crop from sides.
        width = Math.floor((origHeight / ratioHeight) * ratioWidth);
        x = Math.floor((origWidth - width) / 2);
      }

      // Set the new selection.
      custom_crop.widget.setSelection(x, y, (x + width), (y + height));
    }
    else {
      // No ratio requirements, just select the whole image.
      custom_crop.widget.setSelection(0, 0, origWidth, origHeight);
    }

    // Update the widget and stored selection.
    custom_crop.widget.setOptions({hide: false, show: true});
    custom_crop.widget.update();
    custom_crop.updateSelection(null, custom_crop.widget.getSelection());
  }
}

/**
 * Remove the selection.
 */
custom_crop.clearSelection = function() {
  if (custom_crop.croptool) {
    custom_crop.widget.setOptions({hide: true, show: false});
    custom_crop.widget.update();
    custom_crop.updateSelection();

    // Hide clear button.
    $('.custom_crop-clear', custom_crop.croptool).hide();
  }
}

/**
 * When a selection updates write the position and dimensions to the output field.
 *
 * @param image
 *   Reference to the image that is being cropped.
 * @param selection
 *   Object defining the current selection.
 */
custom_crop.updateSelection = function(image, selection) {
  if (custom_crop.croptool) {
    var resized;

    // Update the image reference.
    image = $('img.custom_crop-image', custom_crop.croptool);

    // Get the original width and height.
    var origWidth = custom_crop.parseInt(image.get(0).getAttribute('width'));
    var origHeight = custom_crop.parseInt(image.get(0).getAttribute('height'))

    // Get the instant preview.
    var instantPreview = $('.custom_crop-instantpreview', custom_crop.croptool);

    if (selection && selection.width && selection.height && selection.x1 >= 0 && selection.y1 >= 0) {
      custom_crop.output.val(selection.x1 + '|' + selection.y1 + '|' + selection.width + '|' + selection.height);

      // Update the selection details.
      $('.custom_crop-selection-x', custom_crop.croptool).text(selection.x1);
      $('.custom_crop-selection-y', custom_crop.croptool).text(selection.y1);
      $('.custom_crop-selection-width', custom_crop.croptool).text(selection.width);
      $('.custom_crop-selection-height', custom_crop.croptool).text(selection.height);

      // Update the instant preview.
      if (instantPreview.length) {
        // Calculate the instant preview dimensions.
        resized = custom_crop.resizeDimensions(selection.width, selection.height, instantPreview.data('maxWidth'), instantPreview.data('maxHeight'));

        // Set the new width and height to the preview container.
        instantPreview.css({
          width: resized.width + 'px',
          height: resized.height + 'px'
        });

        // Calculate the resize scale.
        var scaleX = resized.width / selection.width;
        var scaleY = resized.height / selection.height;

        // Update the image css.
        $('img', instantPreview).css({
          width: Math.round(scaleX * origWidth) + 'px',
          height: Math.round(scaleY * origHeight) + 'px',
          marginLeft: '-' + Math.round(scaleX * selection.x1) + 'px',
          marginTop: '-' + Math.round(scaleY * selection.y1) + 'px'
        });
      }
    }
    else {
      custom_crop.output.val('');

      $('.custom_crop-selection-x', custom_crop.croptool).text('-');
      $('.custom_crop-selection-y', custom_crop.croptool).text('-');
      $('.custom_crop-selection-width', custom_crop.croptool).text('-');
      $('.custom_crop-selection-height', custom_crop.croptool).text('-');

      // Reset the instant preview.
      if (instantPreview.length) {
        instantPreview
          .width(instantPreview.data('maxWidth'))
          .height(instantPreview.data('maxHeight'));

        resized = custom_crop.resizeDimensions(origWidth, origHeight, instantPreview.width(), instantPreview.height());

        $('img', instantPreview).css({
          width: resized.width + 'px',
          height: resized.height + 'px',
          marginLeft: '0px',
          marginTop: '0px'
        });
      }
    }

    if (custom_crop.oldSelection) {
      // Show reset button.
      $('.custom_crop-reset', custom_crop.croptool).show();
    }

    // Hide clear button.
    $('.custom_crop-clear', custom_crop.croptool).show();
  }
}

/**
 * A new cropping area was saved to the hidden field, update the default image
 * preview and find the corresponding select option or button and append a css
 * class and text to indicate the crop status.
 *
 * This is a seperate function so it can be triggered after loading.
 *
 * @param element
 *   The hidden field that stores the selection.
 * @param fid
 *   The file id.
 * @param styleName
 *   The image style name.
 */
custom_crop.selectionStored = function(element, fid, styleName) {
  var selection = $(element).val();

  $('.custom_crop-file-' + fid + '-holder img').imagesLoaded(function() {
    var previewHolder = $('.custom_crop-preview-' + fid + '-' + styleName + ' .custom_crop-preview-cropped');
    if (!previewHolder.length) {
      previewHolder = $('.custom_crop-preview-' + fid + ' .custom_crop-preview-cropped');
    }

    var defaultPreview = $('.custom_crop-preview-' + fid + '-' + styleName + ' > img');
    if (!defaultPreview.length) {
      defaultPreview = $('.custom_crop-preview-' + fid + ' > img');
    }

    var toolOpener = $('.custom_crop-style-select-' + fid + " option[value='" + styleName + "'], .custom_crop-style-button-" + fid + ', .custom_crop-style-thumb-' + fid + '-' + styleName + ' .custom_crop-style-thumb-label');
    var hasClass = toolOpener.hasClass('custom_crop-style-cropped');

    if (previewHolder.length && previewHolder.children().length) {
      previewHolder.css({
        width: '0px',
        height: '0px'
      }).html('');
      defaultPreview.css('display', 'block');
    }

    if (selection) {
      if (previewHolder.length) {
        // Get the dimensions of the original preview image and hide it again.
        var maxWidth = custom_crop.parseInt(defaultPreview.width());
        var maxHeight = custom_crop.parseInt(defaultPreview.height());

        if (maxWidth > 0) {
          defaultPreview.css('display', 'none');

          // Get the selected crop area.
          selection = custom_crop.parseStringSelection(selection);

          // Calculate the preview dimensions.
          var resized = custom_crop.resizeDimensions(selection.width, selection.height, maxWidth, maxHeight);

          // Set the new width and height to the cropped preview holder.
          previewHolder.css({
            width: resized.width + 'px',
            height: resized.height + 'px'
          });

          // Calculate the resize scale.
          var scaleX = resized.width / selection.width;
          var scaleY = resized.height / selection.height;

          // Get the original image.
          var originalImage = $('#custom_crop-overlay-' + fid + ' img.custom_crop-image, #custom_crop-inline-' + fid + ' img.custom_crop-image');

          // Calculate the new width and height using the full image.
          resized.width = Math.round(scaleX * custom_crop.parseInt(originalImage.width()));
          resized.height = Math.round(scaleY * custom_crop.parseInt(originalImage.height()));

          // Create and insert the cropped preview.
          previewHolder.append(originalImage.clone().removeClass().css({
            width: resized.width + 'px',
            height: resized.height + 'px',
            marginLeft: '-' + Math.round(scaleX * selection.x1) + 'px',
            marginTop: '-' + Math.round(scaleY * selection.y1) + 'px'
          }));
        }
      }

      if (!hasClass) {
        // Style has been cropped.
        toolOpener.addClass('custom_crop-style-cropped');

        if (toolOpener.is('input')) {
          toolOpener.val(toolOpener.val() + ' ' + Drupal.t('(cropped)'));
        }
        else {
          toolOpener.text(toolOpener.text() + ' ' + Drupal.t('(cropped)'));
        }
      }
    } else if (hasClass) {
      // Style not cropped.
      toolOpener.removeClass('custom_crop-style-cropped');

      if (toolOpener.is('input')) {
        toolOpener.val(toolOpener.val().substr(0, (toolOpener.val().length - Drupal.t('(cropped)').length - 1)));
      }
      else {
        toolOpener.text(toolOpener.text().substr(0, (toolOpener.text().length - Drupal.t('(cropped)').length - 1)));
      }
    }
  });
}

/**
 * Keyboard shortcuts handler.
 *
 * @param e
 *    The event object.
 */
custom_crop.handleKeyboard = function(e) {
  if (custom_crop.croptool) {
    if(e.keyCode == 13) {
      // Enter
      custom_crop.closeCroptool();
    }
    else if(e.keyCode == 27) {
      // Escape
      custom_crop.closeCroptool(true);
    }
  }
}

/**
 * Parse a string defining the selection to an object.
 *
 * @param txtSelection
 *   The selection as a string e.a.: "x|y|width|height".
 * @return
 *   An object containing defining the selection.
 */
custom_crop.parseStringSelection = function(txtSelection) {
  if (txtSelection) {
    var parts = txtSelection.split('|');
    var selection = {
      x1: custom_crop.parseInt(parts[0]),
      y1: custom_crop.parseInt(parts[1]),
      width: custom_crop.parseInt(parts[2]),
      height: custom_crop.parseInt(parts[3])
    };

    selection.x2 = selection.x1 + selection.width;
    selection.y2 = selection.y1 + selection.height;

    return selection;
  }

  return null;
}

/**
 * Parse a textual number to an integer.
 *
 * @param integer
 *   The textual integer.
 * @return
 *   The integer.
 */
custom_crop.parseInt = function(integer) {
  return (parseInt(integer) || 0)
}

/**
 * Calculate new dimensions based upon a maximum width and height.
 *
 * @param width
 *   The current width or an object width all the properties set.
 * @param height
 *   The current height.
 * @param maxWidth
 *   The maximum width.
 * @param maxHeight
 *   The maximum height.
 * @return
 *   An object with the new width and height as properties.
 */
custom_crop.resizeDimensions = function(width, height, maxWidth, maxHeight) {
  if (typeof width == 'object') {
    if (typeof width.maxWidth != 'undefined' && width.maxWidth) {
      maxWidth = width.maxWidth;
    }
    else {
      maxWidth = 9999999;
    }

    if (typeof width.maxHeight != 'undefined' && width.maxHeight) {
      maxHeight = width.maxHeight;
    }
    else {
      maxHeight = 9999999;
    }

    height = width.height;
    width = width.width;
  }
  else {
    if (!maxWidth) {
      maxWidth = 9999999;
    }

    if (!maxHeight) {
      maxHeight = 9999999;
    }
  }

  // Calculate the new width and height.
  if(width > maxWidth) {
    height = Math.floor((height * maxWidth) / width);
    width = maxWidth;
  }

  if(height > maxHeight) {
    width = Math.floor((width * maxHeight) / height);
    height = maxHeight;
  }

  return {
    'width': width,
    'height': height
  };
}

$(document).ready(function() {
  custom_crop.init();

  // Add a blur action to all buttons.
  $('.custom_crop-button').on('mousedown', function() {
    this.blur();
  });

  // Attach behaviors to execute after an ajax call.
  Drupal.behaviors.custom_crop = {
    attach: function(context, settings) {
      // After upload function on image upload.
      $('.ajax-new-content', context).once('custom_crop', function() {
        var element = $(this);

        if (!element.html().length) {
          // If the $form['#file_upload_delta'] is not set or invalid the file module
          // will add an empty <span> as .ajax-new-content element, so we need the
          // previous element to execute the after upload function.
          custom_crop.afterUpload(element.prev().get(0));
        }
        else {
          custom_crop.afterUpload(this);
        }
      });

      // Init function if a modal (Media module) was opened.
      $('.modal-content', context).ready(function() {
        custom_crop.init();
      });
    }
  };
});

})(jQuery);
// Don't clobber any existing jQuery.browser in case it's different

if ( !jQuery.uaMatch ) {
jQuery.uaMatch = function( ua ) {
	ua = ua.toLowerCase();

	var match = /(chrome)[ \/]([\w.]+)/.exec( ua ) ||
		/(webkit)[ \/]([\w.]+)/.exec( ua ) ||
		/(opera)(?:.*version|)[ \/]([\w.]+)/.exec( ua ) ||
		/(msie) ([\w.]+)/.exec( ua ) ||
		ua.indexOf("compatible") < 0 && /(mozilla)(?:.*? rv:([\w.]+)|)/.exec( ua ) ||
		[];

	return {
		browser: match[ 1 ] || "",
		version: match[ 2 ] || "0"
	};
};
}

if ( !jQuery.browser ) {
	matched = jQuery.uaMatch( navigator.userAgent );
	browser = {};

	if ( matched.browser ) {
		browser[ matched.browser ] = true;
		browser.version = matched.version;
	}

	// Chrome is Webkit, but Webkit is also Safari.
	if ( browser.chrome ) {
		browser.webkit = true;
	} else if ( browser.webkit ) {
		browser.safari = true;
	}

	jQuery.browser = browser;
}

