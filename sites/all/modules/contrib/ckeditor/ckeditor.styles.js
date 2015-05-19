/*
Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/*
 * This file is used/requested by the 'Styles' button.
 * The 'Styles' button is not enabled by default in DrupalFull and DrupalFiltered toolbars.
 */
if(typeof(CKEDITOR) !== 'undefined') {
     
   CKEDITOR.addStylesSet( 'drupal', [
        /* Block Styles */
        { name : 'Page Title' , element : 'h1' },
        { name : 'Header' , element : 'h2' },
        //{ name : 'Paragraph Sub Sub Heading' , element : 'h4' },
		    { name : 'Subhead' , element : 'h3'},
        { name : 'Body' , element : 'p' }
]);
}