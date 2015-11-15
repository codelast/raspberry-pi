$(document).ready(function() {
    var maxTimeRangeInputBoxNumber = 10;    // maximum input boxes allowed
    var wrapper = $(".input_fields_wrap");  // fields wrapper
    var timeRangeInputBoxNumber = 1;        // initlal input box number

    // start 5-second timer to call RESTful endpoint
    setInterval(function() {
	$.ajax({
	    url: '/get-time-range',
	    dataType: 'json',
	    success: function(json) {
		$('#time_range').text(json.result);
	    }
	});
    }, 5000);

    $('.js__timepicker').pickatime();

    $("#add_time_range").click(function(e) {
	e.preventDefault();
	if(timeRangeInputBoxNumber < maxTimeRangeInputBoxNumber) {   // max input box allowed
	    timeRangeInputBoxNumber++;  // text box increment
	    $(wrapper).append('<div><input class="js__timepicker" type="text" size="10"/><label> ~ </label><input class="js__timepicker" type="text" size="10"/><a href="#" class="remove_field">Remove</a></div>');  // add input box
	    $('.js__timepicker').pickatime();  // make the dynamically added input box a time picker
	} else {
	    alert('Cannot add more');
	}
    });

    $(wrapper).on("click",".remove_field", function(e){  // user click on remove text
	e.preventDefault(); $(this).parent('div').remove(); timeRangeInputBoxNumber--;
    });

    $("#light_mode_on").click(function() {
	$.ajax({
	    url: '/switch-mode-on',
	    success: function() {
		$("#light_mode_on").prop('disabled', true);    // disable button
		$("#light_mode_off").prop('disabled', false);  // enable button
	    }
	});
    });
    
    $("#light_mode_off").click(function() {
	$.ajax({
	    url: '/switch-mode-off',
	    success: function() {
		$("#light_mode_off").prop('disabled', true);  // disable button
		$("#light_mode_on").prop('disabled', false);  // enable button
	    }
	});
    });

    $("#light_mode_auto").click(function() {
	$.ajax({
	    url: '/switch-mode-auto',
	    success: function() {
		$("#light_mode_on").prop('disabled', false);   // enable button
		$("#light_mode_off").prop('disabled', false);  // enable button
	    }
	});
    });
});
