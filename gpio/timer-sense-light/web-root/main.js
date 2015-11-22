/**
 * This JS is responsible for sending request to the backend program running on RPi & processing 
 * the response from it.
 *
 * @author Darran Zhang @ codelast.com
 */

$(document).ready(function() {
    var maxTimeRangeInputBoxNumber = 10;    // maximum input boxes allowed
    var wrapper = $(".input_fields_wrap");  // fields wrapper
    var timeRangeInputBoxNumber = 1;        // initlal input box number

    $('.js__timepicker').pickatime({
	format: 'HH:i'  // the time text in the time picker will looks like "06:39"
    });

    // dynamically add a pair of time range input boxes
    addTimeRangeInputBox = function() {
	if(timeRangeInputBoxNumber < maxTimeRangeInputBoxNumber) {   // doesn't reach the max input box number
	    timeRangeInputBoxNumber++;  // text box increment
	    $(wrapper).append('<div><input class="js__timepicker" type="text" size="10"/><label> ~ </label><input class="js__timepicker" type="text" size="10"/><a href="#" class="remove_field">Remove</a></div>');  // add input box
	    $('.js__timepicker').pickatime({
		format: 'HH:i'
	    });  // make the dynamically added input box a time picker
	} else {
	    // display a warning message on the page
	    $('#time_range_operation_result').text('Cannot add more').show().fadeOut(3000);
	}
    }

    // when web page just loaded, sync the mode on web page with backend program
    $.ajax({
	url: '/get-mode',
	dataType: 'json',
	success: function(json) {
	    var mode = json.result;  // e.g. 1
	    $("[type=radio]").get(mode).checked = true;
	}
    });

    // when web page just loaded, sync the time range data on web page with backend program
    $.ajax({
	url: '/get-time-range',
	dataType: 'json',
	success: function(json) {
	    var result = json.result;  // e.g. "00:00[SPACE]05:00,21:00[SPACE]23:59"
	    var timeRangeArray = result.split(",");
	    var inputBoxes = $("input.js__timepicker[type='text']");  // get all the time range input boxes

	    // if there are not enough time range input boxes on the web page, we add some first
	    var inputBoxGroupLack = timeRangeArray.length - inputBoxes.length / 2;
	    for (var i = 0; i < inputBoxGroupLack; ++i) {
		addTimeRangeInputBox();
	    }

	    inputBoxes = $("input.js__timepicker[type='text']");
	    var inputBoxIndex = 0;
	    inputBoxes.each(function() {  // iterate all the input boxes & set text for them
		var singleInputBox =$(this);
		var hourAndMinuteArray = timeRangeArray[parseInt(inputBoxIndex / 2)].split(" ");
		singleInputBox.val(hourAndMinuteArray[inputBoxIndex % 2]);
		inputBoxIndex++;
	    });
	}
    });

    $("#add_time_range").click(function(e) {
	e.preventDefault();  // if we want to maintain focus on the input, prevent the default action on "mousedown"
	addTimeRangeInputBox();
    });

    $(wrapper).on("click",".remove_field", function(e) {  // user click on remove text
	e.preventDefault();
	$(this).parent('div').remove();
	timeRangeInputBoxNumber--;
    });

    $("#save_time_range").click(function(e) {
	var inputBoxes = $("input.js__timepicker[type='text']");  // get all the time range input boxes
	var allInputText = '';
	var index = 0;
	inputBoxes.each(  // iterate all the input boxes
	    function() {
		var singleInputBox =$(this);
		allInputText += singleInputBox.val()
		if (index > 0) {
		    allInputText += "\n";
		    index = 0;
		} else {
		    allInputText += "\t";
		    index++;
		}
	    }
	);
	$.ajax({
	    url: '/save-time-range',
	    method: 'POST',
	    data: { "str": allInputText }, 
	    success: function() {
		$('#time_range_operation_result').text('Saved').show().fadeOut(2000);
	    },
	    error: function() {
		$('#time_range_operation_result').text('Error').show().fadeOut(2000);
	    }
	});
    });

    $("[type=radio]").click(function() {
	var checkedValue = $(this).filter(':checked').val();  // get the value of the checked radiobox
	$.ajax({
	    url: '/set-mode',
	    method: 'POST',
	    data: { "mode": checkedValue }, 
	});
    });
});
