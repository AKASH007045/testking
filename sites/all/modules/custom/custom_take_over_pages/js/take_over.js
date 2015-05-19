/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

jQuery(document).ready(function() {
    var querystring = window.location.search;
    if (querystring == '?edit=true') {
        setTimeout(function() {
            jQuery('.region-content ul.tabs-primary-live li.edit-link').click();
        }, 2000);
    }
    // view take over page, All URL-Alias Form update (hide)
    jQuery('.custom-form-tk-node').hide();
    //views-field-nid
    jQuery('#views-form-take-over-pages-page .views-field-nid').hide();
    jQuery('.take-over-pages-form').hide();
    jQuery('.take-over-pages-form .vertical-tabs').hide();
    jQuery('.to-bg-img').hide();
    jQuery('.custom-to-page-nid').hide();
    //jQuery('.form-item-field-run-from-und-0-value2').find('.control-label').text('End date');
    jQuery('.take-over-pages-form .popup-close').click(function() {
        // var r = confirm("Press a button!");
        alertifyreset();
        alertify.confirm("You have unsaved changes. ", function(e) {
            if (e) {
                //var wraper = jQuery(this).parents('div.take-over-pages-form');
                jQuery('.take-over-pages-form').hide();
            } else {
                return;
            }
        });

    });

    //start, Take over page-INLINE edit form
    jQuery('.take-over-pages-link-edit').click(function() {
        jQuery('.icon-close').click();
        var jQuerythis = jQuery(this)
        jQuerythis.next('div.take-over-pages-form').show();
    });
    //start put BG-img in backgroup
    //jQuery('.to-bg-img').find('img').hide();
    //jQuery('.to-bg-img').parents().find('.container-fluid').css('background-image', 'url(' + jQuery('.to-bg-img').find('img').attr('src') + ')');
    //end put BG-img in backgroup  
    //container-fluid
    jQuery('.node-type-take-over-pages .promo-box').find('a.promo-link').click(function(e) {
        e.preventDefault();
    });
    //First remove all the existing click handlers
    jQuery('.node-type-take-over-pages .promo-box').find('a').off('click');
    //then disable the default click behavoiour on anchors
    jQuery('.node-type-take-over-pages .promo-box').find('a').click(function(e) {
        //alert("hello");
        e.preventDefault();
        //
    });
    
    jQuery('.node-take_over_pages-form').addClass('take_over_form');
    jQuery('#custom-add-take-over').addClass('take_over_form');
});