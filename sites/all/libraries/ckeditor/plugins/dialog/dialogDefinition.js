/*
 Copyright (c) 2003-2014, CKSource - Frederico Knabben. All rights reserved.
 For licensing, see LICENSE.md or http://ckeditor.com/license
*/
/**Ckeditor**/
  CKEDITOR.on('dialogDefinition', function( ev )
	{
	console.log("sa");
	   var dialogName = ev.data.name;  
	   var dialogDefinition = ev.data.definition;
			 
	   switch (dialogName) {  
	   case 'image': //Image Properties dialog      
	   dialogDefinition.removeContents('Link');
	   dialogDefinition.removeContents('advanced');
	   break;      
	   case 'link': //image Properties dialog          
	   dialogDefinition.removeContents('advanced');   
	   break;
	   }
	});