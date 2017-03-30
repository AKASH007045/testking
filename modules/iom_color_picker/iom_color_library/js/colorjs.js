// jQuery methods go here...
function update(id) {
 //console.log(id);
 //jQuery("#"+id).colorpicker();
}
jQuery(document).ready(function() {

jQuery("#brand-area input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});
jQuery("#buttons-area input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});
jQuery("#buttons-area2 input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});
jQuery("#topnav-area input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});
jQuery("#subnav-area input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});
jQuery("#promo-area input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});
jQuery("#calendar-area input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});

jQuery("#mobile-styles input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});


jQuery("#calendar input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});

jQuery("#mobile-buttons input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});

jQuery("#custom-invoice-admin-manage-invoice-css input.form-text").each(function() {
    var id = jQuery(this).attr('id');
    if (id !== 'undefined' && id) {
      var div_id = jQuery("#"+ id).closest("div").prop("id");       
      jQuery('#'+ div_id).colorpicker();
    }
});











//jQuery('#colorpicker_nav_item_hover_border').colorpicker();
  
});
