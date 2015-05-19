/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
var selectopen =0;
(function ($) {
  "use strict";

  Drupal.behaviors.imageeditor_imagefield = {};
  Drupal.behaviors.imageeditor_imagefield.attach = function(context, settings) {
    $.each(Drupal.settings.imageeditor_imagefield, function(index, value) {
      
      var field_name = index, editors = value.editors, uploaders = value.uploaders;
      $.each(this.items, function(index, value) {
        
        var $removebutton, $createimage, $imagewidget, options;
        $removebutton = $('input[id^="'+value+'-remove-button"]', context);
        $removebutton = $removebutton.length?$removebutton:$('button[id^="'+value+'-remove-button"]', context);
        $createimage = $('a#'+value+'-imageeditor-source', context);
        if ($removebutton.length && !$removebutton.hasClass('imageeditor-imagefield-processed')) {
          $removebutton.addClass('imageeditor-imagefield-processed');
          if ($removebutton.parents('div.image-widget-data').length) {
            $imagewidget = $removebutton.parents('div.image-widget-data');
          }
          else {
            $imagewidget = $removebutton.parents('tr').find('div.image-widget-data');
          }
          var url = $imagewidget.find('span.file').find('a').attr('href'),
            $element = Drupal.settings.imageeditor_imagefield[field_name].imageeditor_icons_position === 'top' ? $imagewidget.children().first() : $imagewidget.children().last();
          options = {
            editors: editors,
            uploaders: uploaders,
            image: {url: url},
            data: {field_name: field_name, element_id: value},
            $element: $element,
            method: Drupal.settings.imageeditor_imagefield[field_name].imageeditor_icons_position === 'top' ? 'before' : 'after',
            callback: Drupal.imageeditor_imagefield.save
          };
          Drupal.imageeditor.initialize(options);
           var $imageeditor_source = $removebutton.parents('div.image-widget-data');
         //add open aviary editor on upload file.
          var $editors = $imageeditor_source.find('div.editors').find('div');
          //console.log(options);
        //  console.log($imagewidget);
        //  console.log($createimage);
       
          if ($editors.length === 1 &&  selectopen ==0) {
            
            $editors.click();
          }
          else {
            selectopen =0;
          }
        }
        else if ($createimage.length && !$createimage.hasClass('imageeditor-imagefield-processed')) {
          $createimage.addClass('imageeditor-imagefield-processed');
          var $imageeditor_source = $createimage.parents('div.image-widget-data').find('div.filefield-source-imageeditor');
          options = {
            editors: editors,
            uploaders: uploaders,
            data: {field_name: field_name, element_id: value},
            $element: $imageeditor_source.find('div'),
            method: 'replaceWith',
            callback: Drupal.imageeditor_imagefield.save
          };
          if (Drupal.settings.imageeditor_imagefield[field_name].source_imageeditor_image) {
            options.image = {url: Drupal.settings.imageeditor_imagefield[field_name].source_imageeditor_image};
          }
          Drupal.imageeditor.initialize(options);
          // Launch the editor upon clicking on the "Create image" link if there is only one editor.
          var $editors = $imageeditor_source.find('div.editors').find('div');
          if ($editors.length === 1) {
            $createimage.click(function(event) {
              $editors.click();
            });
          }
        }
      });
      //TODO: remove the processed values from Drupal.settings - as the AJAX callback returns all the field values
      //Drupal.settings.imageeditor_imagefield[field_name].items = [];
    });
  };

  Drupal.imageeditor_imagefield = {
    save: function() {
      var edit_id = Drupal.settings.imageeditor.save.element_id.replace(/[0-9]+$/, '');
      // Should we replace the image?
      Drupal.settings.imageeditor.save.replace = Drupal.settings.imageeditor.save.replace || Drupal.settings.imageeditor_imagefield[Drupal.settings.imageeditor.save.field_name].imageeditor_replace;

      // Upload as a new image.
      if (Drupal.settings.imageeditor.save.create || !Drupal.settings.imageeditor.save.replace) {
        // Activate the Remote URL tab.
        Drupal.imageeditor_imagefield.action(edit_id, 'activate_remote_url');
        // Fill in the Remote URL textfield.
        Drupal.imageeditor_imagefield.action(edit_id, 'fill_remote_url');
        // Submit through the Transfer button.
        Drupal.imageeditor_imagefield.action(edit_id, 'submit_transfer');
      }
      // Replace the original image.
      else {
        // Check for the Remote URL source presence on the page.
        if (Drupal.imageeditor_imagefield.action(edit_id, 'check_remote_url')) {
          // Activate the Remote URL tab.
          Drupal.imageeditor_imagefield.action(edit_id, 'activate_remote_url');
          // Fill in the Remote URL textfield.
          Drupal.imageeditor_imagefield.action(edit_id, 'fill_remote_url');
          // Submit through the Remove button.
          $('[id^="' + Drupal.settings.imageeditor.save.element_id + '-remove-button"]').mousedown();
        }
        else {
          // There is no new image upload - so we remove the old one first.
          $('[id^="' + Drupal.settings.imageeditor.save.element_id + '-remove-button"]').mousedown();
          // Upload new image in 0.5 seconds.
          setTimeout(function() {Drupal.imageeditor_imagefield.saveloop(edit_id);}, 500);
        }
      }
    },
    saveloop: function(edit_id) {
      // Check for the Remote URL source presence on the page.
      if (Drupal.imageeditor_imagefield.action(edit_id, 'check_remote_url')) {
        // Activate the Remote URL tab.
        Drupal.imageeditor_imagefield.action(edit_id, 'activate_remote_url');
        // Fill in the Remote URL textfield.
        Drupal.imageeditor_imagefield.action(edit_id, 'fill_remote_url');
        // Submit through the Transfer button.
        Drupal.imageeditor_imagefield.action(edit_id, 'submit_transfer');
      }
      else {
        setTimeout(function() {Drupal.imageeditor_imagefield.saveloop(edit_id);}, 500);
      }
    },
    action: function(edit_id, action) {
      switch (action)  {
        // Check for the Remote URL source presence on the page.
        case 'check_remote_url':
          return $('a[id^="' + edit_id + '"][id$="-remote-source"]').filter(function() {
            var regexp = new RegExp('^' + edit_id + '[0-9]+-remote-source$');
            return regexp.test(this.id);
          }).length;
        case 'activate_remote_url':
          // Activate the Remote URL tab.
          $('a[id^="' + edit_id + '"][id$="-remote-source"]').filter(function() {
            var regexp = new RegExp('^' + edit_id + '[0-9]+-remote-source$');
            return regexp.test(this.id);
          }).click();
          break;
        case 'fill_remote_url':
          // Fill in the Remote URL textfield.
          $('[id^="' + edit_id + '"][id*="-filefield-remote-url"]').filter(function() {
            var regexp = new RegExp('^' + edit_id + '[0-9]+-filefield-remote-url(--[0-9]+)?$');
            return regexp.test(this.id);
          }).val(Drupal.settings.imageeditor.save.image);
          break;
        case 'submit_transfer':
          // Submit through the Transfer button.
          $('[id^="' + edit_id + '"][id*="-filefield-remote-transfer"]').filter(function() {
            var regexp = new RegExp('^' + edit_id + '[0-9]+-filefield-remote-transfer(--[0-9]+)?$');
            return regexp.test(this.id);
          }).mousedown();
          break;
      }
      return true;
    }
  };

})(jQuery);
