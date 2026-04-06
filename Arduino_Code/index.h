//HTML code for web mornitor

String html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Cloud Watcher setup</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            padding: 30px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
        }
        h1 {
            text-align: center;
            color: #333;
            margin-bottom: 30px;
            font-size: 2.5em;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.1);
        }
        .stats-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        .stat-card {
            background: linear-gradient(45deg, #ff6b6b, #ee5a24);
            color: white;
            padding: 20px;
            border-radius: 10px;
            text-align: center;
            box-shadow: 0 10px 20px rgba(0,0,0,0.1);
            transform: translateY(0);
            transition: transform 0.3s ease;
        }
        .stat-card:hover {
            transform: translateY(-5px);
        }
        .stat-value {
            font-size: 2em;
            font-weight: bold;
            margin-bottom: 5px;
        }
        .stat-label {
            font-size: 0.9em;
            opacity: 0.9;
        }
        .chart-container {
            background: white;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 10px 20px rgba(0,0,0,0.1);
            margin-bottom: 20px;
            height: 400px;
            position: relative;
            overflow: hidden;
        }
        .chart-canvas {
            width: 100%;
            height: 100%;
            border: 1px solid #ddd;
            border-radius: 5px;
        }
        .refresh-btn {
            background: linear-gradient(45deg, #4ecdc4, #44a08d);
            color: white;
            border: none;
            padding: 12px 30px;
            border-radius: 25px;
            cursor: pointer;
            font-size: 1em;
            font-weight: bold;
            box-shadow: 0 5px 15px rgba(0,0,0,0.2);
            transition: all 0.3s ease;
            display: block;
            margin: 20px auto;
        }
        .refresh-btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 7px 20px rgba(0,0,0,0.3);
        }
        .last-updated {
            text-align: center;
            color: #666;
            font-style: italic;
            margin-top: 20px;
        }
        .limit-control {
            background: white;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 10px 20px rgba(0,0,0,0.1);
            margin-bottom: 20px;
            text-align: center;
        }
        .limit-input {
            display: inline-flex;
            align-items: center;
            gap: 15px;
            margin-top: 10px;
        }
        .limit-input input {
            padding: 8px 12px;
            border: 2px solid #ddd;
            border-radius: 8px;
            font-size: 16px;
            width: 80px;
            text-align: center;
        }
        .limit-input input:focus {
            outline: none;
            border-color: #ff6b6b;
        }
        .limit-btn {
            background: linear-gradient(45deg, #ff6b6b, #ee5a24);
            color: white;
            border: none;
            padding: 8px 20px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 14px;
            font-weight: bold;
            transition: all 0.3s ease;
        }
        .limit-btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(0,0,0,0.2);
        }
        .limit-status {
            margin-top: 10px;
            font-size: 14px;
            padding: 5px 10px;
            border-radius: 5px;
            display: none;
        }
        .limit-status.success {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
        }
        .limit-status.error {
            background: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🌧️ Cloud Watcher</h1>
        
        <div class="stats-grid">
            <div class="stat-card">
                <div class="stat-value" id="currentTemp">--</div>
                <div class="stat-label">Current SkyTemperature (°C)</div>
            </div>
            <div class="stat-card" style="background: linear-gradient(45deg, #ff9ff3, #f368e0);">
                <div class="stat-value" id="maxTemp">--</div>
                <div class="stat-label">Maximum (12h)</div>
            </div>
            <div class="stat-card" style="background: linear-gradient(45deg, #54a0ff, #2e86de);">
                <div class="stat-value" id="avgTemp">--</div>
                <div class="stat-label">Average (12h)</div>
            </div>
            <div class="stat-card" style="background: linear-gradient(45deg, #3742fa, #2f3542);">
                <div class="stat-value" id="battery">--</div>
                <div class="stat-label">Battery(%)</div>
            </div>
        </div>
        
        <div class="limit-control">
            <h3 style="margin-bottom: 15px; color: #333;">🚨 SkyTemperature Limit Setting</h3>
            <div class="limit-input">
                <label for="tempLimit">Limit:</label>
                <input type="number" id="tempLimit" min="0" max="100" step="0.1" value="40.0">
                <span>°C</span>
                <button class="limit-btn" onclick="updateLimit() ">Update Limit</button>
            </div>
            <div id="limitStatus" class="limit-status"></div>
        </div>
        
        <div class="chart-container">
            <h3 style="text-align: center; margin-bottom: 15px; color: #333;">SkyTemperature History (Last 12 Hours)</h3>
            <canvas id="SkyTemperatureChart" class="chart-canvas"></canvas>
        </div>
        
        <button class="refresh-btn" onclick="updateChart() ">🔄 Refresh Data</button>
        <div class="last-updated" id="lastUpdated">Last updated: --</div>
    </div>

    <script>
        var SkyTemperatureData = [];
        var timeLabels = [];
        var currentTempLimit = 30.0;
        
        function drawChart() {
            var canvas = document.getElementById('SkyTemperatureChart');
            var ctx = canvas.getContext('2d');
            var rect = canvas.getBoundingClientRect();
            canvas.width = rect.width;
            canvas.height = rect.height;
            var width = canvas.width;
            var height = canvas.height;
            
            ctx.clearRect(0, 0, width, height);
            
            if (SkyTemperatureData.length === 0) {
                ctx.fillStyle = '#666';
                ctx.font = '16px Arial';
                ctx.textAlign = 'center';
                ctx.fillText('No data available', width/2, height/2);
                return;
            }
            
            var margin = {top: 40, right: 40, bottom: 80, left: 80};
            var chartWidth = width - margin.left - margin.right;
            var chartHeight = height - margin.top - margin.bottom;
            
            if (chartWidth <= 0 || chartHeight <= 0) return;
            
            var minTemp = Math.min.apply(Math, SkyTemperatureData);
            var maxTemp = Math.max.apply(Math, SkyTemperatureData);
            
            // Include SkyTemperature limit in scale calculation
            minTemp = Math.min(minTemp, currentTempLimit);
            maxTemp = Math.max(maxTemp, currentTempLimit);
            
            var tempRange = maxTemp - minTemp;
            var padding = tempRange > 0 ? tempRange * 0.1 : 1;
            var yMin = minTemp - padding;
            var yMax = maxTemp + padding;
            var yRange = yMax - yMin;
            
            // Draw grid lines
            ctx.strokeStyle = '#e0e0e0';
            ctx.lineWidth = 1;
            ctx.font = '12px Arial';
            ctx.fillStyle = '#666';
            
            // Horizontal grid lines (SkyTemperature)
            var numYLines = 5;
            for (var i = 0; i <= numYLines; i++) {
                var y = margin.top + (chartHeight * i / numYLines);
                var temp = yMax - (yRange * i / numYLines);
                ctx.beginPath();
                ctx.moveTo(margin.left, y);
                ctx.lineTo(margin.left + chartWidth, y);
                ctx.stroke();
                ctx.textAlign = 'right';
                ctx.fillText(temp.toFixed(1) + '°C', margin.left - 10, y + 4);
            }
            
            // Vertical grid lines (relative time)
            var xStep = chartWidth / (SkyTemperatureData.length > 1 ? SkyTemperatureData.length - 1 : 1);
            var labelStep = Math.max(1, Math.floor(SkyTemperatureData.length / 8));
            for (var i = 0; i < SkyTemperatureData.length; i += labelStep) {
                var x = margin.left + i * xStep;
                ctx.beginPath();
                ctx.moveTo(x, margin.top);
                ctx.lineTo(x, margin.top + chartHeight);
                ctx.stroke();
                
                // Calculate relative time in hours (0 to -6)
                var relativeHours = -(SkyTemperatureData.length - 1 - i) / 60; // assuming 1 reading per minute
                var timeLabel = relativeHours.toFixed(1) + 'h';
                
                ctx.save();
                ctx.translate(x, height - margin.bottom + 20);
                ctx.rotate(-0.3);
                ctx.textAlign = 'center';
                ctx.fillText(timeLabel, 0, 0);
                ctx.restore();
            }
            
            // Draw SkyTemperature limit line
            var limitY = margin.top + chartHeight - ((currentTempLimit - yMin) / yRange) * chartHeight;
            if (limitY >= margin.top && limitY <= margin.top + chartHeight) {
                ctx.strokeStyle = '#ff0000';
                ctx.lineWidth = 2;
                ctx.setLineDash([5, 5]);
                ctx.beginPath();
                ctx.moveTo(margin.left, limitY);
                ctx.lineTo(margin.left + chartWidth, limitY);
                ctx.stroke();
                ctx.setLineDash([]);
                
                // Draw limit label
                ctx.fillStyle = '#ff0000';
                ctx.font = 'bold 12px Arial';
                ctx.textAlign = 'left';
                ctx.fillText('Limit: ' + currentTempLimit.toFixed(1) + '°C', margin.left + 10, limitY - 5);
            }
            
            // Draw SkyTemperature line (no data points)
            if (SkyTemperatureData.length > 0) {
                ctx.strokeStyle = '#00ffff';
                ctx.lineWidth = 3;
                ctx.beginPath();
                
                for (var i = 0; i < SkyTemperatureData.length; i++) {
                    var x = margin.left + i * xStep;
                    var y = margin.top + chartHeight - ((SkyTemperatureData[i] - yMin) / yRange) * chartHeight;
                    
                    if (i === 0) {
                        ctx.moveTo(x, y);
                    } else {
                        ctx.lineTo(x, y);
                    }
                }
                ctx.stroke();
            }
            
            // Draw axes
            ctx.strokeStyle = '#333';
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(margin.left, margin.top);
            ctx.lineTo(margin.left, margin.top + chartHeight);
            ctx.moveTo(margin.left, margin.top + chartHeight);
            ctx.lineTo(margin.left + chartWidth, margin.top + chartHeight);
            ctx.stroke();
            
            // Draw axis labels
            ctx.fillStyle = '#333';
            ctx.font = '14px Arial';
            ctx.textAlign = 'center';
            ctx.fillText('Time (hours ago)', width/2, height - 10);
            ctx.save();
            ctx.translate(15, height/2);
            ctx.rotate(-Math.PI/2);
            ctx.fillText('SkyTemperature (°C)', 0, 0);
            ctx.restore();
        }
        
        function loadSkyTemperatureLimit() {
            fetch('/getlimit')
                .then(function(response) { return response.json(); })
                .then(function(data) {
                    currentTempLimit = data.limit;
                    document.getElementById('tempLimit').value = currentTempLimit.toFixed(1);
                    console.log('SkyTemperature limit loaded:', currentTempLimit);
                })
                .catch(function(error) {
                    console.error('Error loading SkyTemperature limit:', error);
                });
        }
        
        function updateLimit() {
            var newLimit = parseFloat(document.getElementById('tempLimit').value);
            var statusDiv = document.getElementById('limitStatus');
            
            if (isNaN(newLimit) || newLimit < 0 || newLimit > 100) {
                showStatus('Invalid SkyTemperature limit. Please enter a value between 0 and 100°C', 'error');
                return;
            }
            
            var formData = new FormData();
            formData.append('limit', newLimit.toString());
            
            fetch('/setlimit', {
                method: 'POST',
                body: formData
            })
            .then(function(response) { return response.json(); })
            .then(function(data) {
                if (data.status === 'success') {
                    currentTempLimit = data.limit;
                    showStatus('SkyTemperature limit updated to ' + currentTempLimit.toFixed(1) + '°C', 'success');
                    drawChart(); // Redraw chart with new limit line
                } else {
                    showStatus('Error: ' + data.message, 'error');
                }
            })
            .catch(function(error) {
                console.error('Error updating SkyTemperature limit:', error);
                showStatus('Error updating SkyTemperature limit', 'error');
            });
        }
        
        function showStatus(message, type) {
            var statusDiv = document.getElementById('limitStatus');
            statusDiv.textContent = message;
            statusDiv.className = 'limit-status ' + type;
            statusDiv.style.display = 'block';
            
            // Hide status after 3 seconds
            setTimeout(function() {
                statusDiv.style.display = 'none';
            }, 3000);
        }
        
        function updateChart() {
            fetch('/data')
                .then(function(response) { return response.json(); })
                .then(function(data) {
                    console.log('Data received:', data);
                    SkyTemperatureData = data.SkyTemperatures;
                    // No need to process time labels as we're using relative time
                    
                    console.log('SkyTemperature data:', SkyTemperatureData);
                    drawChart();
                    
                    if (data.SkyTemperatures.length > 0) {
                        var temps = data.SkyTemperatures;
                        var current = temps[temps.length - 1];
                        var max = Math.max.apply(Math, temps);
                        var avg = temps.reduce(function(a, b) { return a + b; }, 0) / temps.length;
                        var bat = data.battery[0];
                        
                        document.getElementById('currentTemp').textContent = current.toFixed(1);
                        document.getElementById('maxTemp').textContent = max.toFixed(1);
                        document.getElementById('avgTemp').textContent = avg.toFixed(1);
                        document.getElementById('battery').textContent = bat.toFixed(1);                    }
                    
                    document.getElementById('lastUpdated').textContent = 
                        'Last updated: ' + new Date().toLocaleString();
                })
                .catch(function(error) {
                    console.error('Error fetching data:', error);
                    var canvas = document.getElementById('SkyTemperatureChart');
                    var ctx = canvas.getContext('2d');
                    var rect = canvas.getBoundingClientRect();
                    canvas.width = rect.width;
                    canvas.height = rect.height;
                    ctx.clearRect(0, 0, canvas.width, canvas.height);
                    ctx.fillStyle = '#ff0000';
                    ctx.font = '16px Arial';
                    ctx.textAlign = 'center';
                    ctx.fillText('Error loading data', canvas.width/2, canvas.height/2);
                });
        }
        
        window.onload = function() {
            loadSkyTemperatureLimit(); // Load SkyTemperature limit first
            setTimeout(updateChart, 500);
            window.addEventListener('resize', function() {
                setTimeout(drawChart, 200);
            });
            setInterval(updateChart, 3000);
        };
    </script>
</body>
</html>
)";
