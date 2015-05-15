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

function show_schedule($this){ 
  if($this){
    //Remove the div
    jQuery("body").find("div#plandesccontainer").remove();
    jQuery("body").find('div.ttlamt').html("$0.00");
    jQuery.ajax({
      cache: false,
      type: 'POST',
      url: "/plandescription/" + jQuery("#payment_plan_id").val(),
      beforeSend: function() {
        jQuery("#edit-submit").attr('disabled','disabled');
        jQuery("body").find("div.form-item-payment-plan-id").after('<div id="plansloading"><img src="/sites/all/modules/custom/custom_invoice/images/loader_big.gif" /></div>');
      },
      context: document.body,
      dataType: "json"
    })
    .done(function(json, textStatus, jqXHR) { 
      jQuery("#edit-submit").removeAttr('disabled');
      jQuery("body").find("#plansloading").remove();
      if(json && json.item_list){ 
        jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">&nbsp;</div>');
        jQuery("body").find('#plandesccontainer').html(json.item_list);
        if(json.amount_due_today){
          jQuery("body").find('div.ttlamt').html("");
          jQuery("body").find('div.ttlamt').html("$"+json.amount_due_today);
          
          jQuery("body").find('div#div_amount_due_today').remove();
          jQuery("body").find('input#edit-email').after('<div id="div_amount_due_today" style="visibility:hidden;"><input id="amount_due_today" type="text" name="amount_due_today" value="'+json.amount_due_today+'"></div>');
          //document.getElementById("amount_due_today").value = json.amount_due_today;
        }
      }
      else{
        jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">No description available for this plan</div>');
      }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {
      jQuery("#edit-submit").removeAttr('disabled');
      jQuery("body").find("#plansloading").remove();
      
      jQuery("body").find("div.form-item-payment-plan-id div.DivSelectyze").after('<div id="plandesccontainer" class="form-item-payment-plan-id form-item form-group">No description available for this plan</div>');
    });
  }
}
