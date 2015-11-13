$(document).ready(function() {
    // start 1-second timer to call RESTful endpoint
    setInterval(function() {
	$.ajax({
	    url: '/get-time-range',
	    dataType: 'json',
	    success: function(json) {
		$('#time-range').text(json.result + '% ');
	    }
	});
    }, 1000);

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
