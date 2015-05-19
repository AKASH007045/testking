(function($){
  Drupal.behaviors.plandescGetDesc = {
    attach: function (context) {
        var $this = "#payment_plan_id";
        show_schedule($this);
      $("#payment_plan_id").on("change", function(){
        show_schedule(this);
      });
    }
  }
})(jQuery);

function check_terms_page(){ 
  if(jQuery("form#custom-invoice-payment-request").length>0){
    jQuery("form#custom-invoice-payment-request #edit-submit").attr("disabled", true);
    var terms = false;
    if(jQuery("form#custom-invoice-payment-request").find('input[type="checkbox"]').is(':checked')){
        terms = true;
    }
    
    if(terms){ 
      jQuery("body").find("#edit-submit").removeAttr('disabled');
    }
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
      url: "/plandescription/" + jQuery("#payment_plan_id").val(),
      beforeSend: function() {
        //disable submit button and show loader image
        jQuery("#edit-submit").attr('disabled','disabled');
        jQuery("body").find("div.form-item-payment-plan-id").after('<div id="plansloading"><img src="/sites/all/modules/custom/custom_invoice/images/loader_big.gif" /></div>');
      },
      context: document.body,
      dataType: "json"
    })
    .done(function(json, textStatus, jqXHR) {          
      //disable submit button for duration of ajax call  
      //jQuery("#edit-submit").removeAttr('disabled');
     // check_terms_page();
      jQuery("body").find("#plansloading").remove();
      
      //show plan description
      jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plansbreif">&nbsp;</div>');
      if(json && json.desc){                
        jQuery("body").find('#plansbreif').html(json.desc);
      }
      else{
        jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plansbreif">No description available</div>');
      }
      
      //Set input amount payable value
      if(json && json.amount_due_today){
        jQuery("body").find('#edit-amount').attr('value', json.amount_due_today);
        
        //update amount due today static value
        jQuery("body").find('div.ttlamt').html("");
        jQuery("body").find('div.ttlamt').html(json.amount_due_detail);
        
        //setup the hidden field for payment amount submission
        jQuery("body").find('div#div_amount_due_today').remove();
        jQuery("body").find('input#edit-email').after('<div id="div_amount_due_today" style="visibility:hidden;"><input id="amount_due_today" type="text" name="amount_due_today" value="'+json.amount_due_today+'"></div>');
      }
      else{
        jQuery("body").find('#edit-amount').attr('value', "0");
        jQuery("body").find('input#edit-email').after('<div id="div_amount_due_today" style="visibility:hidden;"><input id="amount_due_today" type="text" name="amount_due_today" value="0.00"></div>');
      }
      
      //show payment breakup
      if(json && json.item_list){ 
        jQuery("body").find("div.form-item-payment-plan-id #plansbreif").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">&nbsp;</div>');
        jQuery("body").find('#plandesccontainer').html(json.item_list);
      }
      else{
        jQuery("body").find("div.form-item-payment-plan-id #plansbreif").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">No Payment breakup available for this plan</div>');
      }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {
      //enable pay button
      jQuery("#edit-submit").removeAttr('disabled');
      jQuery("body").find("#plansloading").remove();
      
      jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">No description available for this plan</div>');
    });
  }
}
//jQuery(document).ready(function(){
//  jQuery('form#custom-invoice-payment-request input[type="text"]').on('keyup', function(){ 
//    check_terms_page();
//  });
//  jQuery('form#custom-invoice-payment-request input[type="checkbox"]').on('click', function(){ 
//    check_terms_page();
//  });
//});