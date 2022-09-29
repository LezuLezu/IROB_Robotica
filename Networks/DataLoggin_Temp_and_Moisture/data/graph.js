var dataArray = [
    [1664364640,21.70,50.00], 
    [1664364646,21.70,50.00], 
    [1664364652,21.70,50.00],
    [1664364658,21.70,51.00],
    [1664364664,21.70,50.00],
    [1664364670,21.70,50.00],
];

var defaultZoomTime = 24*60*60*1000; // 1 day
var minZoom = -6; // 22 minutes 30 seconds
var maxZoom = 8; // ~ 8.4 months

var zoomLevel = 0;
var viewportEndTime = new Date();
var viewportStartTime = new Date();

// loadCSV(); // Download the CSV data, load Google Charts, parse the data, and draw the chart


/*
Structure:

    loadCSV
        callback:
        parseCSV
        load Google Charts (anonymous)
            callback:
            updateViewport
                displayDate
                drawChart
*/

/*
               |                    CHART                    |
               |                  VIEW PORT                  |
invisible      |                   visible                   |      invisible
---------------|---------------------------------------------|--------------->  time
       viewportStartTime                              viewportEndTime

               |______________viewportWidthTime______________|

viewportWidthTime = 1 day * 2^zoomLevel = viewportEndTime - viewportStartTime
*/

google.charts.load('current', { 'packages': ['line', 'corechart'] });
parseCSV(dataArray);
function parseCSV(dataArray) {
    for (var i = 0; i < dataArray.length; i++) {
        dataArray[i][0] = new Date(parseInt(dataArray[i][0]) * 1000);
        
    }
    return dataArray;
}
console.log();

google.setOnLoadCallback(updateViewport); // call updateViewport when the page loads

function drawChart() {
    var data = new google.visualization.DataTable();
    data.addColumn('datetime', 'UNIX');
    data.addColumn('number', 'Temperature');    
    data.addColumn('number', 'Humidity');

    data.addRows(dataArray);
    console.log(data);

    var options = {
        title: 'Temperature and Humidity',
        curveType: 'function',
        legend: { position: 'bottom' },
        height: 360,

        // create two vertical axes (one for temperature, one for humidity)
        vAxes: {    
            0: {title: 'Temperature (°C)', viewWindow: {min: 0, max: 50}},
            1: {title: 'Humidity (%)', viewWindow: {min: 0, max: 100}}
        },
        // create a horizontal axis (time)
        hAxis: {
            title: 'Time',
            viewWindow: {
                min: viewportStartTime,
                max: viewportEndTime
            }
        }
        

    };

    // var options = {
    //     curveType: 'function',

    //     height: 360,

    //     legend: { position: 'top' },
    //     hAxis: {
    //         title: 'Time',
    //         viewWindow: {
    //             min: viewportStartTime,
    //             max: viewportEndTime
    //         },
    //         gridlines: {
    //             count: -1,
    //             units: {
    //                 days: { format: ['MMM dd'] },
    //                 hours: { format: ['HH:mm', 'ha'] },
    //             }
    //         },
    //         minorGridlines: {
    //             units: {
    //                 hours: { format: ['hh:mm:ss a', 'ha'] },
    //                 minutes: { format: ['HH:mm a Z', ':mm'] }
    //             }
    //         }
    //     },
    //     vAxis: [
    //         {title: 'Temperature'},
    //         {title: 'Humidity'}
    //     ],
    //     series: {
    //         0: {targetAxisIndex: 0},
    //         1: {targetAxisIndex: 1}
    //     }
    // };

    var chart = new google.charts.Line(document.getElementById('chart_div'));

    chart.draw(data, options);

    var dateselectdiv = document.getElementById("dateselect");
    dateselectdiv.style.visibility = "visible";

    var loadingdiv = document.getElementById("loading");
    loadingdiv.style.visibility = "hidden";
}

function displayDate() { // Display the start and end date on the page
    var dateDiv = document.getElementById("date");

    var endDay = viewportEndTime.getDate();
    var endMonth = viewportEndTime.getMonth();
    var startDay = viewportStartTime.getDate();
    var startMonth = viewportStartTime.getMonth()
    if (endDay == startDay && endMonth == startMonth) {
        dateDiv.textContent = (endDay).toString() + "/" + (endMonth + 1).toString();
    } else {
        dateDiv.textContent = (startDay).toString() + "/" + (startMonth + 1).toString() + " - " + (endDay).toString() + "/" + (endMonth + 1).toString();
    }
}

document.getElementById("prev").onclick = function() {
    viewportEndTime = new Date(viewportEndTime.getTime() - getViewportWidthTime()/3); // move the viewport to the left for one third of its width (e.g. if the viewport width is 3 days, move one day back in time)
    updateViewport();
}
document.getElementById("next").onclick = function() {
    viewportEndTime = new Date(viewportEndTime.getTime() + getViewportWidthTime()/3); // move the viewport to the right for one third of its width (e.g. if the viewport width is 3 days, move one day into the future)
    updateViewport();
}

document.getElementById("zoomout").onclick = function() {
    zoomLevel += 1; // increment the zoom level (zoom out)
    if(zoomLevel > maxZoom) zoomLevel = maxZoom;
    else updateViewport();
}
document.getElementById("zoomin").onclick = function() {
    zoomLevel -= 1; // decrement the zoom level (zoom in)
    if(zoomLevel < minZoom) zoomLevel = minZoom;
    else updateViewport();
}

document.getElementById("reset").onclick = function() {
    viewportEndTime = new Date(); // the end time of the viewport is the current time
    zoomLevel = 0; // reset the zoom level to the default (one day)
    updateViewport();
}
document.getElementById("refresh").onclick = function() {
    viewportEndTime = new Date(); // the end time of the viewport is the current time
    loadCSV(); // download the latest data and re-draw the chart
}

document.body.onresize = drawChart;

function updateViewport() {
    viewportStartTime = new Date(viewportEndTime.getTime() - getViewportWidthTime());
    displayDate();
    drawChart();
}
function getViewportWidthTime() {
    return defaultZoomTime*(2**zoomLevel); // exponential relation between zoom level and zoom time span
                                           // every time you zoom, you double or halve the time scale
}

