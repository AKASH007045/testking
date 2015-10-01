(function($) {
    $.fn.bootstrapValidator.validators.validateExpDate = {
        /**
         * Check if input value is empty or not
         *
         * @param {BootstrapValidator} validator The validator plugin instance
         * @param {jQuery} $field Field element
         * @param {Object} options
         * @returns {Boolean}
         */
        validate: function(validator, $field, options) {
          var vexd = $field.val();
          var mm = vexd.substring(0, 2);
          var yy = vexd.substring(2, 4);
          dateObj = new Date();
          var month = dateObj.getUTCMonth();
          month = month + 1;
          var day = dateObj.getUTCDate();
          var year = dateObj.getUTCFullYear();
          year = year.toString();
          year = year.substring(2, 4);
          console.log(day + " " + month + " " + year);
          if(yy<year){ return false; }
          if( (mm<=0) || (mm>12) ){ return false; }
          return true;
        }
    };
}(window.jQuery));
jQuery(document).ready(function() {  
  jQuery('#custom-invoice-payment-request').bootstrapValidator({
    message: 'This value is not valid',
    fields: {
      first_name: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
      last_name: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
      Avs_address: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
      Avs_state: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
      Avs_postal_code: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
          zipCode: {
            message: '&nbsp'
          },
        }
      },
      ctype: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
      cc_num: {
        validators: {
          creditCard: {
            message: '&nbsp'
          },
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
      exp_date: {
        validators: {
          notEmpty: {
            message: '&nbsp;'
          },
          stringLength: {
            min: 4,
            message: '&nbsp;'
          },
          digits: {
            message: '&nbsp;'
          },
          validateExpDate: {
             message: '&nbsp;'
          }
        }
      },
      cin: {
        validators: {
          cvv: {
            message: '&nbsp'
          },
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
      email: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
          emailAddress: {
            message: '&nbsp'
          }
        }
      },
      amount: {
        validators: {
          notEmpty: {
            message: '&nbsp'
          },
        }
      },
    }
  });
  jQuery('#custom-invoice-payment-request #edit-submit').click(function() {
    jQuery('#custom-invoice-payment-request').bootstrapValidator();
  });
  
  jQuery('#custom-invoice-payment-request #edit-cc-num').bind('keyup change', function() {
    cardType = '';
    if(jQuery(this).val().length >= 3) {
      cardType = getCreditCardType(jQuery(this).val());
      var cardText = '';
      if (cardType) {
        switch(cardType) {
          case 'VI':
            cardText = 'Visa';
            break;
          case 'MC':
            cardText = 'MasterCard';
            break;
          case 'DI':
            cardText = 'Discover';
            break;
          case 'AE':
            cardText = 'American Express';
            break;
          default:
            cardText = '';
        }
        //var cardArray = new Array("Master Card", "Visa", "Discover", "American Express" );
        jQuery('select#edit-ctype').css("display","block").css("visibility", "hidden");
        jQuery('select#edit-ctype').find('option').removeAttr('selected');
        jQuery('select#edit-ctype').find('option').each(function(){
          var optval = jQuery(this).attr('value');
          if(optval==cardType){
            jQuery(this).attr("selected", "selected");
          }
        });
        jQuery('select#edit-ctype').val(cardType);
        jQuery('.form-item-ctype a.selectyzeValue').text(cardText);
      }
      else {
        jQuery('#custom-pawp-tmapi-request-form #edit-ctype').val('');
        jQuery('.form-item-ctype a.selectyzeValue').text('');
      }
    }
  });
  jQuery('#custom-invoice-payment-request #edit-ctype').change(function() {
    jQuery('#custom-pawp-tmapi-request-form #edit-cc-num').val('');
  });
});