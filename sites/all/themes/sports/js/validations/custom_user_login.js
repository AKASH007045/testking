jQuery(document).ready(function() {
  jQuery("#edit-name").focus();
  jQuery('#user-login').bootstrapValidator({
    message: 'This value is not valid',
    feedbackIcons: {
      validating: 'glyphicon glyphicon-refresh'
    },
    fields: {
      name: {
        message: 'The Acct ID is not valid',
        validators: {
          notEmpty: {
            message: 'Please enter your Account ID'
          },
        }
      },
      pass: {
        validators: {
          notEmpty: {
            message: 'Please enter your password'
          },
        }
      }
    }
  });
  //Validate the form manually
  jQuery('#user-login #edit-submit').click(function() {
    jQuery('#user-login').bootstrapValidator();
  });
});