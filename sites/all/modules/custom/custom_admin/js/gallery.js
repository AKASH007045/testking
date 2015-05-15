function hideSelect($this){
	if(jQuery($this).val()=='Image'){
		jQuery($this).parents('tr').find('div.field-name-field-gallery-videos').hide();
	}
	else if(jQuery($this).val()=='Video'){
		jQuery($this).parents('tr').find('div.field-name-field-gallery-videos').show();
	}
}
jQuery(document).ready(function(){
	jQuery("#gallery-node-form #field-gallery-values select.form-select").each(function(){
		hideSelect(this);
	});
	
	jQuery("#gallery-node-form #field-gallery-values select.form-select").on('change', function(){
		hideSelect(this);
	});
});
