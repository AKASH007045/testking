function tiles_form_action(fid) {
  jQuery('#' + fid).find('[name=op]').attr("disabled", true);
  var textCounter = false;
  jQuery('#' + fid).find('input:text').keyup(check_submit);
  jQuery('#' + fid).find('input:text, input:checkbox').change(check_submit);
  jQuery('#' + fid + ' select').change(check_submit);
  jQuery('#' + fid + ' .imageeditor-widget-item').click(check_submit);
}

function check_submit(e, fid1) {
    ttextCounter = true;
    if ( typeof fid1 !== "undefined") {
        var fid=fid1.parents('.home-page-tile').attr('id');//code
    }
    else {
        var fid=jQuery(this).parents('.node-form').attr('id');
    }
    var textCounter = false;
    var textCounter1 = false;
    var $this =  jQuery('#' + fid).find('[name=title]');
    if ($this && $this.val().length > 0) {
        textCounter = true;
    }
    else {
        textCounter = false;
    }
    
    var $this =  jQuery('#' + fid).find('[name=field_image\\[und\\]\\[0\\]\\[fid\\]]');
    if ($this && $this.val().length > 0 && $this.val() >0) {
        textCounter1 = true;
    }
    else {
        textCounter1 = false;
    }
  
    var ttt = true;
    if (textCounter && textCounter1) {
        ttt = false;
    }
    jQuery('#' + fid).find('[name=op]').attr("disabled", ttt);
}

function hidepromoSelect($this, $id){
	if(jQuery($this).val()=='regular'){
	    jQuery('#' + $id).find('div.field-name-field-countdown-title').hide();
	    jQuery('#' + $id).find('div.field-name-field-date').hide();
	}
	else if(jQuery($this).val() == 'countdown'){
	    jQuery('#' + $id).find('div.field-name-field-countdown-title').show();
	    jQuery('#' + $id).find('div.field-name-field-date').show();
	}
}

// Funcion called before unloading of the page.
function confirmUnloading() {
  if ((ttextCounter == true || jQuery('.quickedit-changed').length) && ttextCounter1) {
    return "You have unsaved changes.";
  }
}

function alertifyreset () {
  alertify.set({
    labels : {
      ok     : "Discard changes",
      cancel : "Save"
    },
    delay : 5000,
    buttonReverse : false,
    buttonFocus   : "ok"
  });
}