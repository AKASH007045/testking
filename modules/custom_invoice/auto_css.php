<?php

$invoice_css = " /*-------------
  Please don't add any css directly from here,
  Invoice auotomation script started------------*/

.page-invoice .container-fluid .page-header,
.page-invoice .container-fluid .page-header > div { color: $site_title_color; } /* page title text color*/

table#invoices-list-table th:nth-child(2), table#invoices-list-table th:nth-child(3), 
#invoices-list-past-table th:nth-child(2), #invoices-list-past-table th:nth-child(3), 
#invoices-list-pending-table th:nth-child(2), #invoices-list-pending-table th:nth-child(3) {
background-color: $invoice_list_header_bg_color;
color:$invoice_list_text_color;
}

.amountduewrap .amountduettl .pull-right a:hover,
.payment-amount-box .make-payment-wrapper button#edit-submit:hover,
.page-invoice-payment button.form-submit,
.amountduewrap .amountduettl .pull-right a:focus,
.payment-amount-box .make-payment-wrapper button#edit-submit:focus,
.page-invoice-payment button.form-submit {
  color: $buttons_color;
  background: $buttons_back_hover_color; /*#5b4810*/
  border: 1px solid $buttons_back_hover_border_color; /*rgba(0, 0, 0, 0.75)*/
}
.page-invoice .amountduewrap {
 background-image: url('$order_invoice_watermark');
    background-color: rgba(158, 158, 158, 0.35);
    background-position: 9% center;
    background-repeat: no-repeat;
}

.season-invoice-detail .btn-default,
.page-invoice-payment button.form-submit,
.page-invoice .season-invoice-detail .pull-right .btn-default:focus,
.page-invoice-payment .btn-default.return-home,
.page-invoice-payment .btn-default.return-home:focus,
body .container-fluid > section.main-wrap .email-button.btn.btn-default,
body .container-fluid > section.main-wrap .email-button.btn.btn-default:focus {
    border: 1px solid $email_button_border;/* #030303 */
    -webkit-box-shadow: 0px 1px 1px $email_button_box_shadow;
    box-shadow: 0px 1px 1px $email_button_box_shadow; /* $email_buttons_box_shadow */
    color: $email_buttons_color; /* #ffffff */
    $email_button_bg_gradiant
}

.page-invoice .season-invoice-detail .pull-right a:hover,
.page-invoice-payment .btn-default.return-home:hover, 
body .container-fluid > section.main-wrap .email-button.btn.btn-default:hover {
  background: $email_button_bg_hover;
  color: $email_buttons_color; /* #ffffff */
  border: 1px solid $email_button_border;
}
.page-invoice .btn-light,
.make-payment-wrapper button.form-submit,
.page-invoice .pull-right .btn-default:focus,
.payment-amount-box .make-payment-wrapper button#edit-submit,
.payment-amount-box .make-payment-wrapper button#edit-submit:focus, 
.page-invoice-payment button.form-submit,
.page-invoice-payment button.form-submit,
.page-buy-order-detail-payment .payment-amount-box .payment-submit button,
.page-buy-order-detail-payment .payment-amount-box .payment-submit button:focus {
    /* Pay Button */
    border: 1px solid $buttons_border_color; /* rgba(0, 0, 0, 0.75); */
    -webkit-box-shadow: 0px 1px 0px #dedede inset; /*$buttons_boxshadow_color*/
    box-shadow: 0px 1px 0px #dedede inset; /* $buttons_boxshadow_color */    
    $buttons_default_color_gradiant
    color: $buttons_color; /* #ffffff */
}
.page-invoice .btn-light:hover {color: $buttons_color;}



.payment-thankyou-wrapper .return-home,
.payment-unsuccessful .return-home {
  border: 1px solid #030303; /*email-buttons-border*/
  -webkit-box-shadow: 0px 1px 1px #5b5b5b inset;
  box-shadow: 0px 1px 1px #5b5b5b inset; /*email-buttons-box-shadow*/
  color: $buttons_color; /* #ffffff */  
  background-image: $email_button_bg_gradiant;
}

.payment-thankyou-wrapper .return-home:hover, .payment-thankyou-wrapper .return-home:focus,
.payment-unsuccessful .return-home:hover,
.payment-unsuccessful .return-home:focus {
  background: $email_button_bg_hover;
}

.page-invoice-payment-success .payment-thankyou-wrapper h1, 
.page-invoice-payment-success .payment-thankyou-wrapper h2,
.page-invoice--payment-success .payment-thankyou-wrapper h1,
.page-invoice--payment-success .payment-thankyou-wrapper h2,
.page-invoice-details-payment-success .payment-thankyou-wrapper h1,
.page-invoice-details-payment-success .payment-thankyou-wrapper h2, 
.page-buy .payment-thankyou-wrapper h1,
.page-buy .payment-thankyou-wrapper h2,
.invoice-not-awl h1,
.infoPanelwrap h2 span,
.page-invoice .page-header,
.season-invoice-detail h3,
.page-invoice .page-header, 
.page-invoice-details .page-header, 
.page-buy .page-header,
.season-invoice-detail .ticketTable tr.head td  {
  color: $invoice_page_color;
}

.infoPanelwrap .dedicateserrep {
  background-color: $invoice_reps_bg;  /*#000000;*/
  border: 1px solid $invoice_reps_bg; /*#000000;*/
  color: #ffffff;
}

.infoPanelwrap .dedicateserrep h5.dsr {
  border-bottom: 4px solid #ffffff;
}

.infoPanelwrap .dedicateserrep .imgpics {
  background-color: #008d96;
}

.infoPanelwrap .contDetail a, 
.infoPanelwrap .contDetail a:hover, 
.infoPanelwrap .contDetail a:focus {
  color: #000000;
  text-decoration: none;
}
.page-invoice .css3 .selectyzeValue:after {
color: $dropdown_arrow_color;
}
.page-invoice .css3 li a:hover {background-color:$dropdown_arrow_color;}

/*-------------Eof Invoice auotomation script------------*/
  ";
