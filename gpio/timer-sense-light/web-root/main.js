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
});
