/**
 * @license Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
 * For licensing, see LICENSE.html or http://ckeditor.com/license
 */

CKEDITOR.editorConfig = function( config ) {
	// Define changes to default configuration here. For example:
	// config.language = 'fr';
	// config.uiColor = '#AADC6E';
  config.font_names = 'Arial/Arial;Oswald/oswaldregular;';
  config.extraPlugins = 'lineheight';
  config.line_height="100%/1;125%/1.25;150%/1.5;175%/1.75;200%/2;225%/2.25;250%/2.5;275%/2.75;300%/3;325%/3.25;350%/3.5;375%/3.75;400%/4;425%/4.25;450%/4.5;475%/4.75;500%/5";
};
/**Ckeditor Upload tab remove in link property**/
CKEDITOR.on( 'dialogDefinition', function( ev )
{		
	var dialogName = ev.data.name;
	var dialogDefinition = ev.data.definition;
	
	if ( dialogName == 'link')
	{						
		dialogDefinition.removeContents( 'upload' );			
	}
});
/**Ckeditor Upload tab remove in link property**/