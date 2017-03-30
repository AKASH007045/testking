<?php

/**
 * @file
 * Contains all description to generate PDF document from raw HTML content.
 */

/**
 * Generate HTML of a given node.
 *
 * @return bool
 *   TRUE if PDF is successfully generated and FALSE if it isn't.
 */
function pdf_using_mpdf_generate_pdf($node) {
  if (!node_access('view', $node)) {
    drupal_set_message(t('You are not authorized to generate PDF for this page.'), 'warning');
    drupal_goto(urlencode('node') . '/' . $node->nid);
    return;
  }
  if (empty($node)) {
    drupal_get_messages('error');
    drupal_set_message(t('PDF cannot be generated for this path.'), 'error');
    return;
  }

  // Checking mPDF library existence.
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $pdf_using_mpdf_pdf_filename = variable_get('pdf_using_mpdf_pdf_filename', PDF_USING_MPDF_PDF_DEFAULT_FILENAME);
    $pdf_using_mpdf_pdf_filename = token_replace($pdf_using_mpdf_pdf_filename, array('node' => $node));

    $view = node_view($node);
    $html = drupal_render($view);

    _pdf_using_mpdf_generator($html, $pdf_using_mpdf_pdf_filename);
	
	
	 // set document information

  $mpdf = new mPDF('UTF-8', $format);
  $mpdf->setAutoTopMargin = 'stretch';
  $mpdf->setAutoBottomMargin = 'stretch';
  $mpdf->SetHTMLHeader('<div>sadsdsdsdsf<img src="http://....../bandeau_pdf.jpg" width="800px" ></div>');
  $mpdf->SetHTMLFooter('<div class="pdf_date" style="color:black; text-align:left;" >{ AQQWQ	WDATE j-m-Y}</div><div class="pdf_pagination" style="color:black; text-align:right;" >{PAGENO}</div>');
  }
  else {
    drupal_set_message(t('mPDF library is not included. Please check your "sites/all/libraries" directory or " /sites/your-domain-name/libraries/ directory " or "!default_module_path" directory.', array(
      '!default_module_path' => drupal_get_path('module', 'pdf_using_mpdf'),
    )), 'warning');
    drupal_goto(urlencode('node') . '/' . $node->nid);
    return;
  }
}