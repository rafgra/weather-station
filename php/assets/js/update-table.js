  $(function() {

      if (!Array.prototype.last) {
          Array.prototype.last = function() {
              return this[this.length - 1];
          };
      };

      $.getJSON("api/all?iso=2&limit=86400&order=desc", function(response) {
          loadData(response);
          setInterval(update, 10000);
      });

      function loadData(response) {
          var c = [];
          var data = response[0];
          display(data);

          $.each(response, function(index, data) {
              c.push(tableRow(data));
          });

          $('#data-table tbody').html(c.join(""));
      }

      function update() {
          $.getJSON("api/last?iso=2", function(data) {
              display(data);
              updateTable(data);
          });
      }

      function updateTable(data) {
          $('#data-table tbody tr:last').remove();
          $('#data-table tbody').prepend(tableRow(data));
      }

      function tableRow(data) {
          return '<tr><td>' + data.date + '</td><td>' + parseFloat(data.temperature).toFixed(2) + '</td><td>' + parseFloat(data.pressure).toFixed(2) + '</td><td>' + parseFloat(data.humidity).toFixed(2) + '</td><td>' + parseFloat(data.illuminance).toFixed(2) + '</td><td>' + parseFloat(data.dewpoint).toFixed(2) + '</td></tr>';
      }

      function display(data) {
        $temperatureBox = $('#temperature').closest(".data-container");
        if(parseFloat(data.temperature) < 0){
          $temperatureBox.switchClass("red", "blue", 1000, "easeInOutQuad" );
        }else{
          $temperatureBox.switchClass("blue", "red", 1000, "easeInOutQuad" );
        }
          $('#date').html(data.date);
          $('#temperature').html(parseFloat(data.temperature).toFixed(2));
          $('#pressure').html(parseFloat(data.pressure).toFixed(2));
          $('#humidity').html(parseFloat(data.humidity).toFixed(2));
          $('#illuminance').html(parseFloat(data.illuminance).toFixed(2));
          $('#dewpoint').html(parseFloat(data.dewpoint).toFixed(2));
      }
  });
