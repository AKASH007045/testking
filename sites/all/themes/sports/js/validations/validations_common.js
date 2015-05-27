/**
 * Function to get Credit card type information
 * @param accountnumber account number of the user
 * @return card type code
 */
function getCreditCardType(accountNumber) {
  //start without knowing the credit card type
  var result = "";
  //first check for MasterCard
  if (/^5[1-5]/.test(accountNumber)) {
    result = "MC";
  }
  //then check for Visa
  else if (/^4/.test(accountNumber)) {
    result = "VI";
  }
  //then check for AmEx
  else if (/^3[47]/.test(accountNumber)) {
    result = "AE";
  }
  //then check for AmEx
  else if (/^6(?:011|5)/.test(accountNumber)) {
    result = "DI";
  }
  return result;
}

//function validateExpDate(vexd, format){
//  if(format=="mmyy"){
//    var mm = vexd.substring(0, 2);
//    var yy = vexd.substring(2, 4);
//    if( (mm<=0) && (mm>12) ){ return false; }
//    return true;
//  }  
//  return false;    
//}  