(function ($) {
  Drupal.behaviors.dynamic_dropdown_demo =  {
    attach: function(context, settings) {
         
        $('#webform-client-form-1188 #edit-submitted-exchange-to-game'+' option:not(:first)').remove().end();
         
        $("#webform-client-form-1188 #edit-submitted-exchange-from-game").change(function(){
          var nid = $('#webform-client-form-1188 #edit-submitted-exchange-from-game option:selected').val();
         
          $("#webform-client-form-1188 #edit-submitted-exchange-to-game").html('Listing models...');
                 
          $.get(Drupal.settings.basePath +'?q=get-game-list/' + nid, null, updateModel);
            return false;
         
        });
         
        var updateModel = function(result) {
                $("#webform-client-form-1188 #edit-submitted-exchange-to-game").html(result.html_output);
        }
         
    }
  };
})(jQuery);
