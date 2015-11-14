$(document).ready(function() {
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
