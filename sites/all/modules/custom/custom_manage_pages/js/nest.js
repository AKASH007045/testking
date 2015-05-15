jQuery(document).ready(function()
{
    var updateOutput = function(e)
    {
        var list   = e.length ? e : jQuery(e.target),
            output = list.data('output');
        if (window.JSON) {
            output.val(window.JSON.stringify(list.nestable('serialize')));//, null, 2));
        } else {
            output.val('JSON browser support required for this demo.');
        }
        
        var jsonValue = window.JSON.stringify(list.nestable('serialize'));
        //jsonValue = jsonValue.replace(/\"/g, "&quot;"); 
        jQuery("#navlist_input").attr('value',"");
        jQuery("#navlist_input").attr('value', jsonValue);
    };
    
    
    jQuery("fieldset.form-wrapper").each( function(){ if(!jQuery(this).hasClass("collapsible")) { jQuery(this).hide(); } } );
    
    jQuery("li.dd-item a.showdraft").click(function(){
      jQuery(this).closest("li.dd-item").find("fieldset.form-wrapper").slideToggle();
      jQuery(this).toggleClass("open");
    });
    // activate Nestable for list 1
    jQuery('#nestable').nestable({
        group: 1
    }).on('change', updateOutput);

    //Output initial serialised data
    updateOutput(jQuery('#nestable').data('output', jQuery('#nestable-output')));
    
    //manage pages show hide list items
    var usertype = jQuery.cookie('mp_usertype');
    if(usertype){
      jQuery("div.select select").val(usertype);
    }
    else{
      jQuery("div.select select").val('anon');
      usertype = 'anon';
    }
    
    jQuery("#nestable ol > li.loggedin").hide();
    listshowhide(usertype);
    
    jQuery("div.select select").on('change', function(){
      var val = jQuery(this).val();
      jQuery.cookie('mp_usertype', val);
      listshowhide(val);
    });
    
});

function listshowhide(value){
  if(value=="anon"){
    jQuery("#nestable ol > li.loggedin").hide();
    jQuery("#nestable ol > li.anonymous").show();    
  }
  else{
    jQuery("#nestable ol > li.anonymous").hide();
    jQuery("#nestable ol > li.loggedin").show();    
  }
}

