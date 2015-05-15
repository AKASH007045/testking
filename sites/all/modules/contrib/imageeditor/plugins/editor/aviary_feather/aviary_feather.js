/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.aviaryfeather = function() {
    if (typeof(Aviary) !== 'undefined' && typeof(Drupal.settings.imageeditor.aviary_feather) !== 'undefined') {
      var options = Drupal.settings.imageeditor.aviary_feather.options;
      options.onSave = function(imageID, newURL) {
        Drupal.settings.imageeditor.save.image = newURL;
        if (Drupal.settings.imageeditor.aviary_feather.parameters.closeonsave) {
          Drupal.imageeditor.aviaryfeather.close();
        }
      };
      options.onClose = function(isDirty) {
        selectopen = 1;
        if (typeof Drupal.settings.imageeditor.save.image !== 'undefined') {
           
          Drupal.imageeditor.save();
        }
        selectopen =0;
      };
      options.onError = function(errorObj) {
        selectopen =0;
        alert(errorObj.message);
      };
      options.tools = 'all';
      if (Drupal.settings.imageeditor_imagefield.field_image && Drupal.settings.imageeditor_imagefield.field_image.imageeditor_preset &&  Drupal.settings.imageeditor_imagefield.field_image.imageeditor_preset !='') {
        var perset = Drupal.settings.imageeditor_imagefield.field_image.imageeditor_preset;
        options.cropPresets = [['Image size ' + perset, perset]];
        options.cropPresetsStrict = true;
        //options.initTool = 'crop';
        options.forceCropPreset = ['Image size', perset];
        options.forceCropMessage = 'Please crop your photo to this size:';
      }
      return new Aviary.Feather(options);
    }
    else {
      return {};
    }
  }();

  Drupal.imageeditor.editors.aviary_feather = Drupal.imageeditor.editors.aviary_feather || {};
  Drupal.imageeditor.editors.aviary_feather.initialize = function($imageeditor_div) {
    $imageeditor_div.find('div.aviary-feather').not('.imageeditor-processed').addClass('imageeditor-processed').click(function(event) {
      event.preventDefault();
      event.stopPropagation();
      var data = $imageeditor_div.data();
      Drupal.imageeditor.save = data.callback;
      Drupal.settings.imageeditor.save = data;
      Drupal.settings.imageeditor.save.create = 0;
      var options = {
        image: $('<img />').attr('src', data.url).get(0)
        //url: data.url
      };
      // Some magic for highres images - doesn't work.
      /*if (Drupal.settings.imageeditor.aviary_feather.options.timestamp) {
        options.hiresUrl = data.url;
        options.timestamp = Drupal.settings.imageeditor.aviary_feather.options.timestamp;
        options.signature = Drupal.settings.imageeditor.aviary_feather.options.signature;
      }*/
     
      Drupal.imageeditor.aviaryfeather.launch(options);
    });
  };

})(jQuery);
