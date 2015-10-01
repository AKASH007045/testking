/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor.editors.mailru_camera = Drupal.imageeditor.editors.mailru_camera || {};
  Drupal.imageeditor.editors.mailru_camera.initialize = function($imageeditor_div) {
    $imageeditor_div.find('div.imageeditor-mailru-camera').not('.imageeditor-processed').addClass('imageeditor-processed').click(function(event) {
      event.preventDefault();
      event.stopPropagation();
      var data = $imageeditor_div.data();
      Drupal.settings.imageeditor.save = data;
      // Define image saving function.
      Drupal.imageeditor.save = data.callback;

      var codename = $(this).data('codename');
      var options = Drupal.settings.imageeditor[codename].options;
      if (typeof(data.url) !== 'undefined') {
        Drupal.settings.imageeditor.save.create = 0;
        var filename = data.origurl.replace(new RegExp('.*/', 'g'), '');
        $.cookie('imageeditor_filename', filename, {expires: 7, path: Drupal.settings.basePath});
      }
      else {
        Drupal.settings.imageeditor.save.create = 1;
      }
      Drupal.imageeditor[Drupal.settings.imageeditor[codename].launch_type].show(options, $(this).attr('title'));
    });
  };

})(jQuery);