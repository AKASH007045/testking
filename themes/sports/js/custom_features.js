jQuery(document).ready(function(){
	jQuery('button.global-button').click(function(e){
		e.preventDefault();return false;
	});
	
	jQuery('button.global-button a').click(function(){
		alert('anchor tag clicked');
	});
});