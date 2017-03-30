(function($) {
    Drupal.behaviors.GetcreditcardQuery = {
        attach: function(context) {
            var $this = "#edit-choose-card";
            var $this = $("#edit-choose-card");
            //console.log('entry works');
            cc_query_details_implement($this);
            $("#edit-choose-card").on("change", function() {
                cc_query_details_implement(this);
            });
        }
    }
})(jQuery);



function cc_query_details_implement($this) {
    if ($this) {
        selected_value = jQuery("#edit-choose-card").val();
        jQuery('.form-item-ctype').removeClass('disableSelect');

        jQuery("body").find('#edit-ctype').val("");
        jQuery("body").find('#edit-cc-num').attr('value', "");
        jQuery("body").find('#edit-exp-date').attr('value', "");
        jQuery("body").find('#edit-name-first').attr('value', "");
        jQuery("body").find('#edit-name-last').attr('value', "");
        jQuery("body").find('#edit-street-addr-1').attr('value', "");
        jQuery("body").find('#edit-city').attr('value', "");
        jQuery("body").find('#edit-state').attr('value', "");
        jQuery("body").find('#edit-zip').attr('value', "");

        if (selected_value == "") {
            jQuery("body").find('#edit-ctype').val("");
            jQuery('.form-item-ctype a.selectyzeValue').text('- Select -');
            jQuery("body").find('#edit-cc-num').attr('value', "");
            jQuery("body").find('#edit-cc-num').val("");
            jQuery("body").find('#edit-cc-num').attr('readonly', false);
            return;
        }

        jQuery.ajax({
                cache: false,
                type: 'POST',
                url: "/cc_query/" + selected_value,
                beforeSend: function() {
                    //Show loader image
                    if (jQuery("#cc-loading").length <= 0) {
                        jQuery("body").find("div.card_loader").after('<div id="cc-loading"><img src="/sites/default/files/loader_big.gif" /></div>');
                    }
                },
                context: document.body,
                dataType: "json"
            })
            .done(function(json, textStatus, jqXHR) {
                jQuery("body").find("#cc-loading").remove();
                jQuery('#custom-invoice-payment-request #edit-cc-num,#edit-cin,#edit-exp-date').val('');
                jQuery("body").find('.form-group').removeClass('has-error');
                if (json && json.data_mask) {
                    //fill all the value in proper area.
                    checkCard(json.cc_type);
                    jQuery("body").find('#edit-ctype').val(json.cc_type);
                    jQuery('.form-item-ctype').addClass('disableSelect');
                    jQuery('#custom-invoice-payment-request #edit-cc-num').val(json.data_mask);
                    jQuery("body").find('#edit-cc-num').attr('readonly', true);
                    jQuery("body").find('.form-item-cc-num').addClass('element-invisible');                    
                   // jQuery("body").find('#edit-exp-date').attr('value', json.cc_exp);
                     jQuery("body").find('#edit-exp-date').val(json.cc_exp);
                    jQuery("body").find('#edit-name-first').attr('value', json.cc_name_first);
                    jQuery("body").find('#edit-name-last').attr('value', json.cc_name_last);
                    jQuery("body").find('#edit-street-addr-1').attr('value', json.cc_address);
                    //jQuery("body").find('#edit-city').attr('value', json.data_mask);
                    //jQuery("body").find('#edit-state').attr('value', json.data_mask);
                    jQuery("body").find('#edit-zip').attr('value', json.cc_postal_code);
                } else if (selected_value == 'new_card' && json) {
                    checkCard(json.cc_type);
                    jQuery("body").find('#edit-cc-num').attr('readonly', false);
                    jQuery("body").find('.form-item-cc-num').removeClass('element-invisible');
                    jQuery("body").find('#edit-name-first').attr('value', json.name_first);
                    jQuery("body").find('#edit-name-last').attr('value', json.name_last);
                    jQuery("body").find('#edit-street-addr-1').attr('value', json.street_addr_1);
                    jQuery("body").find('#edit-city').attr('value', json.city);
                    jQuery("body").find('#edit-state').attr('value', json.state);
                    jQuery("body").find('#edit-zip').attr('value', json.zip);
                }

            })
            .fail(function(jqXHR, textStatus, errorThrown) {
                //
                jQuery("body").find("#cc-loading").remove();
            });
    }
}


function checkCard(cardType) {
    if (jQuery('body').hasClass('page-invoice-payment')) {
        var isVisa = (jQuery('.form-item-ctype select#edit-ctype option[value="VI"]').text()) ? true : false;
        var isMasterCard = (jQuery('.form-item-ctype select#edit-ctype option[value="MC"]').text()) ? true : false;
        var isAmex = (jQuery('.form-item-ctype select#edit-ctype option[value="AE"]').text()) ? true : false;
        var isDiscover = (jQuery('.form-item-ctype select#edit-ctype option[value="DI"]').text()) ? true : false;
    }


    if (cardType && cardType == "") {
        return;
    } else {
        var cardText = '';
        if (cardType) {
            if (cardType == 'VI' && isVisa) {
                cardText = 'Visa';
            } else if (cardType == 'MC' && isMasterCard) {
                cardText = 'MasterCard';
            } else if (cardType == 'AE' && isAmex) {
                cardText = 'American Express';
            } else if (cardType == 'DI' && isDiscover) {
                cardText = 'Discover';
            } else {
                cardText = '- Select -';
            }
            jQuery('select#edit-ctype').css("display", "block").css("visibility", "hidden");
            jQuery('select#edit-ctype').find('option').removeAttr('selected');
            jQuery('select#edit-ctype').find('option').each(function() {
                var optval = jQuery(this).attr('value');
                if (optval == cardType) {
                    jQuery(this).attr("selected", "selected");
                }
            });
            jQuery('select#edit-ctype').val(cardType);
            jQuery('.form-item-ctype a.selectyzeValue').text(cardText);
        } else {
            jQuery('#custom-invoice-payment-request #edit-ctype, #custom-str-invoice-payment-request #edit-ctype').val('');
            jQuery('.form-item-ctype a.selectyzeValue').text('- Select -');
        }
    }
}
