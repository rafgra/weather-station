$(function() {
  if (!Array.prototype.last) {
    Array.prototype.last = function() {
      return this[this.length - 1];
    };
  };
    var chart = AmCharts.makeChart("chart", {
        "type": "serial",
        "theme": "none",
        "pathToImages": "assets/img/",
        "dataProvider": {},
        "legend": {
            "useGraphSettings": true
        },
        "valueAxes": [{
            "id": "v1",
            "axisColor": "#FF0040",
            "axisThickness": 2,
            "gridAlpha": 0,
            "axisAlpha": 1,
            "position": "right"
        }, {
            "id": "v2",
            "axisColor": "#40FF00",
            "axisThickness": 2,
            "gridAlpha": 0,
            "axisAlpha": 1,
            "position": "left"
        }, {
            "id": "v3",
            "axisColor": "#0040FF",
            "axisThickness": 2,
            "gridAlpha": 0,
            "offset": 50,
            "axisAlpha": 1,
            "position": "left"
        }, {
            "id": "v4",
            "axisColor": "#FFC100",
            "axisThickness": 2,
            "gridAlpha": 0,
            "offset": 100,
            "axisAlpha": 1,
            "position": "left"
        }],
        "graphs": [{
            "valueAxis": "v1",
            "type": "step",
            "lineThickness": 3,
            "lineColor": "#FF0040",
            "negativeLineColor": "#0040FF",
            "title": "Temperatura [\xB0C]",
            "balloonText": "[[value]]\xB0C",
            "valueField": "temperature",
            "fillAlphas": 0
        }, {
            "valueAxis": "v2",
            "lineColor": "#40FF00",
            "type": "smoothedLine",
            "lineThickness": 2,
            "title": "Ciśnienie [hPa]",
            "balloonText": "[[value]] hPa",
            "valueField": "pressure",
            "dashLength": 5,
            "fillAlphas": 0
        }, {
            "valueAxis": "v3",
            "lineColor": "#0040FF",
            "type": "smoothedLine",
            "lineThickness": 2,
            "title": "Wilgotność [%]",
            "balloonText": "[[value]]%",
            "valueField": "humidity",
            "dashLength": 5,
            "fillAlphas": 0
        }, {
            "valueAxis": "v4",
            "lineColor": "#FFC100",
            "type": "step",
            "title": "Jasność [lx]",
            "balloonText": "[[value]] lx",
            "valueField": "illuminance",
            "fillAlphas": 0.08
        }],
        "chartScrollbar": {},
        "chartCursor": {
            "cursorPosition": "mouse",
            "categoryBalloonDateFormat": "DD/MM/YYYY JJ:NN:SS"
        },
        "categoryField": "date",
        "categoryAxis": {
            "parseDates": true,
            "minPeriod": "ss",
            "dateFormats": [{
                period: 'fff',
                format: 'JJ:NN:SS'
            }, {
                period: 'ss',
                format: 'JJ:NN:SS'
            }, {
                period: 'mm',
                format: 'JJ:NN'
            }, {
                period: 'hh',
                format: 'JJ:NN'
            }, {
                period: 'DD',
                format: 'DD/MM'
            }, {
                period: 'WW',
                format: 'DD/MM'
            }, {
                period: 'MM',
                format: 'MM'
            }, {
                period: 'YYYY',
                format: 'YYYY'
            }],
            "axisColor": "#DADADA",
            "minorGridEnabled": true
        }
    });

    chart.addListener("dataUpdated", zoomChart);
    zoomChart();



    function zoomChart() {
        chart.zoomToIndexes(chart.dataProvider.length - 48, chart.dataProvider.length - 1);
    }

    function update() {
        $.getJSON("api/last?iso=1&hours=1", function(last) {
          var t = chart.dataProvider.last();
          if(t.date != last.date){
            chart.dataProvider.push(last);
            chart.dataProvider.shift();
            chart.validateData();
          }
        });
    }

    $.getJSON("api/hours?iso=1&limit=172800", function(data) {
        chart.dataProvider = data;
        chart.validateData();
        setInterval(update, 10000);
    });




});
