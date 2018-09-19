(function($){
  Drupal.behaviors.plandescGetDesc = {
    attach: function (context) {
      jQuery("body").find("div.form-item-payment-plan-id").after('<div id="plansloading"><img src="/sites/default/files/loader_big.gif" /></div>');
      window.onload = function(){
        var $this = "#payment_plan_id";
        jQuery("body").find("#plansloading").remove();
        show_schedule($this);
        $("#payment_plan_id").on("change", function(){
          show_schedule(this);
        });
      }
    }
  }
})(jQuery);

function check_terms_page(){
  if(jQuery("form#custom-invoice-payment-request").find('input[type="checkbox"]').length>0){
    jQuery("form#custom-invoice-payment-request #edit-submit").attr("disabled", true).addClass('disabled_button');
    var terms = false;
    if(jQuery("form#custom-invoice-payment-request").find('input[type="checkbox"]').is(':checked')){
        terms = true;
    }

    if(terms){
      jQuery("body").find("#edit-submit").removeAttr('disabled');
	  jQuery('form#custom-invoice-payment-request button#edit-submit').removeClass('disabled_button');
    }
  }
  else {
     jQuery("body").find("#edit-submit").removeAttr('disabled').removeClass("disabled_button");
  }
}

function show_schedule($this){
  if($this){
    //Remove the div
    jQuery("body").find('#plansbreif').remove();
    jQuery("body").find("div#plandesccontainer").remove();
    jQuery("body").find('div.ttlamt').html("$0.00");

    jQuery.ajax({
      cache: false,
      type: 'POST',
      url: "/plandescription/" + jQuery("#payment_plan_id").val() + '/'+ jQuery("#tm_invoice_id").val(),
      beforeSend: function() {
        //disable submit button and show loader image
        jQuery("#edit-submit").attr('disabled','disabled');
		if(jQuery("#plansloading").length <= 0) {
      jQuery("body").css("overflow",'hidden');
			jQuery("body").find("div.form-item-payment-plan-id").after('<div id="plansloading"><img src="/sites/default/files/loader_big.gif" /></div>');
		}
      },
      context: document.body,
      dataType: "json"
    })
    .done(function(json, textStatus, jqXHR) {
      //disable submit button for duration of ajax call
      //jQuery("#edit-submit").removeAttr('disabled');
     // check_terms_page();
	 //console.log(json);
      jQuery("body").css("overflow",'scroll');
      jQuery("body").find("#plansloading").remove();

      //show plan description
      jQuery("body").find('#plansbreif').remove();
      jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plansbreif">&nbsp;</div>');
      if(json && json.desc){
        jQuery("body").find('#plansbreif').html(json.desc);
      }
      else{
        jQuery("body").find('#plansbreif').html("&nbsp;");//No description available
      }

      //Set input amount payable value
      if(json && json.amount_due_today){
        jQuery("body").find('#edit-amount').attr('value', json.amount_due_today);
        //enable pay button
        //jQuery("#edit-submit").removeAttr('disabled').removeClass("disabled_button");
      	check_terms_page();
        //update amount due today static value
        jQuery("body").find('div.ttlamt').html("");
        jQuery("body").find('div.ttlamt').html(json.amount_due_detail);

        //setup the hidden field for payment amount submission
        jQuery("body").find('div#div_amount_due_today').remove();
        jQuery("body").find('input#edit-email').after('<div id="div_amount_due_today" style="visibility:hidden;height:1px;"><input id="amount_due_today" type="text" name="amount_due_today" value="'+json.amount_due_today+'"></div>');
      }
      else{
        jQuery("body").find('#edit-amount').attr('value', "0");
        jQuery("body").find('input#edit-email').after('<div id="div_amount_due_today" style="visibility:hidden;height:1px;"><input id="amount_due_today" type="text" name="amount_due_today" value="0.00"></div>');
      }

      //show payment breakup
      jQuery("body").find('#plandesccontainer').remove();
      jQuery("body").find("div.form-item-payment-plan-id #plansbreif").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">&nbsp;</div>');
      if(json && json.item_list){
        jQuery("body").find('#plandesccontainer').html(json.item_list);
      }
      else if(json && json.suppress){
        jQuery("body").find('#plandesccontainer').html('');
      }
      else{
        jQuery("body").find('#plandesccontainer').html("No Payment breakup available for this plan");
      }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {
      //enable pay button
      //jQuery("#edit-submit").removeAttr('disabled').removeClass("disabled_button");
      check_terms_page();
      jQuery("body").find("#plansloading").remove();

      jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">No description available for this plan</div>');
    });
  }
}
jQuery(document).ready(function(){
	// jQuery('form#custom-invoice-payment-request input[type="text"]').on('keyup', function(){
	// 	check_terms_page();
	// });
	jQuery('form#custom-invoice-payment-request input[type="checkbox"]').on('click', function(){
		check_terms_page();
	});
	if(jQuery("form#custom-invoice-payment-request").find('input[type="checkbox"]').length>0){
		jQuery("form#custom-invoice-payment-request #edit-submit").addClass('disabled_button');
	}
});

/*  Dynamic Payment Plans  */
jQuery(document).ready(function(){
	if(jQuery('body').hasClass('page-invoice-payment')){
		var isVisa = (jQuery('.form-item-ctype select#edit-ctype option[value="VI"]').text()) ? true : false;
		var isMasterCard = (jQuery('.form-item-ctype select#edit-ctype option[value="MC"]').text()) ? true : false;
		var isAmex = (jQuery('.form-item-ctype select#edit-ctype option[value="AE"]').text()) ? true : false;
		var isDiscover = (jQuery('.form-item-ctype select#edit-ctype option[value="DI"]').text()) ? true : false;
		console.log("Visa:" + isVisa + "; MasterCard" + isMasterCard + "; AmericanExpress" + isAmex + "; Discover" + isDiscover);
	}

//Change credit card type based on the value filled for credit card number
    jQuery('#custom-invoice-payment-request #edit-cc-num, #custom-str-invoice-payment-request #edit-cc-num').bind('keyup change', function () {
        cardType = '';
        if (jQuery(this).val().length >= 3) {
            cardType = getCreditCardType(jQuery(this).val());
            var cardText = '';
            if (cardType) {
				if(cardType == 'VI' && isVisa){
					cardText = 'Visa';
				}else if(cardType == 'MC' && isMasterCard){
					cardText = 'MasterCard';
				}else if(cardType == 'AE' && isAmex){
					cardText = 'American Express';
				}else if(cardType == 'DI' && isDiscover){
					cardText = 'Discover';
				}else{
					cardText = '- Select -';
				}
                //var cardArray = new Array("Master Card", "Visa", "Discover", "American Express" );
                jQuery('select#edit-ctype').css("display", "block").css("visibility", "hidden");
                jQuery('select#edit-ctype').find('option').removeAttr('selected');
                jQuery('select#edit-ctype').find('option').each(function () {
                    var optval = jQuery(this).attr('value');
                    if (optval == cardType) {
                        jQuery(this).attr("selected", "selected");
                    }
                });
                jQuery('select#edit-ctype').val(cardType);
                jQuery('.form-item-ctype a.selectyzeValue').text(cardText);
            }
            else {
                jQuery('#custom-invoice-payment-request #edit-ctype, #custom-str-invoice-payment-request #edit-ctype').val('');
                jQuery('.form-item-ctype a.selectyzeValue').text('- Select -');
            }
        }
    });
    //Empty credit card number field
    jQuery('#custom-invoice-payment-request #edit-ctype, #custom-str-invoice-payment-request #edit-ctype').change(function () {
        jQuery('#custom-invoice-payment-request #edit-cc-num, #custom-str-invoice-payment-request #edit-cc-num').val('');
    });
});
