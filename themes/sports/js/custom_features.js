jQuery(document).ready(function(){
	jQuery('button.global-button').click(function(e){
		e.preventDefault();return false;
	});
	
	jQuery('button.global-button a').click(function(){
		alert('anchor tag clicked');
	});
	
	if(window.location.href.indexOf("payment") > -1){
		jQuery('.page-invoice-payment .payment-amount-box input.form-checkbox').attr('checked', false);
		if(jQuery('.make-payment-wrapper input.form-checkbox').length > 0){
			jQuery('.make-payment-wrapper input.form-checkbox').attr('checked', false);
		}else{
			var isError = jQuery('.personal-info .has-error').length;
			if(isError == 0){
				jQuery('.make-payment-wrapper button#edit-submit').attr('disabled', false);
			}
		}
	}
	
	
	var invoice_payment	=	jQuery('.page-invoice .form-item-choose-card select#edit-choose-card').on('change', function(){
		var invoice_pay_text	=	jQuery('.page-invoice .form-item-choose-card .css3 a.selectyzeValue').text();
		var check_existing_card	=	(invoice_pay_text == '- Select -' || invoice_pay_text == 'Add New Card') ? 0 : 1;
		if(!check_existing_card){
			jQuery('input#edit-cc-num').removeClass('required');
		}
	});
	
});