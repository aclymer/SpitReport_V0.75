var DEBUG = true;	// Set 'true' for console messages

var spot_id;
var d = new Date();
var hr = d.getHours();
if (DEBUG === true) console.log(d,hr);
var nearby_spot_id;
var county;
var water_temp;

function fetchNearby(pos) {
	if (DEBUG === true) console.log("Fetching Nearby...");
	var response;
	var coordinates = pos.coords;
	var req = new XMLHttpRequest();
	req.open('GET', "http://api.spitcast.com/api/spot/nearby?longitude="+coordinates.longitude+"&latitude="+coordinates.latitude, false);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			response = JSON.parse(req.responseText);
			if (response && response.length > 0) {
				var surfResult = response;
				spot_id = nearby_spot_id = surfResult[0].spot_id;
				if (DEBUG === true) console.log("Nearest Spot Id: " + spot_id);
			}
		} 
		else {
			if (DEBUG === true) console.log("fetchNearby:HTTP " + req.status + " error");
			fetchNearby(pos);
		}
	};
	req.onloadend = function() {
		if (DEBUG === true) console.log("Done.");
		fetchSurf(spot_id, "refresh");	
	};
	req.send();
}

function fetchCounty(Id) {
	if (DEBUG === true) console.log("Fetching County...");
	var response;
	var req = new XMLHttpRequest();
	if (DEBUG === true) console.log("fetchCounty...");
	req.open('GET', "http://api.spitcast.com/api/spot/all", false);
	req.onload = function(e) {
		if (req.readyState == 4) {
			if (req.status == 200) {
				response = JSON.parse(req.responseText);
				for (var i = 0; i < response.length; i++) {
					if (response[i].spot_id == Id) {
						county = response[i].county_name;
						var county_name = [];
						for (var j = 0; j < county.length; j++) {
							if (county[j] == " ") {county_name.push("-");}
							else {county_name.push(county[j]);}
						}
						county = county_name.join("");
						county = county.toLowerCase();
						if (DEBUG === true) console.log("County: " + county);
						fetchWater(county);
					}
				}
				
			}
			else {
				if (DEBUG === true) console.log("fetchCounty:HTTP " + req.status + " error");
			}
		}
	};
	req.send(null);
	req.onloadend = function() {
		if (DEBUG === true) console.log("Done.");
	};
}

function fetchNeighbor(Id, dir) {
	if (DEBUG === true) console.log("Fetching Neighbor...");
	var response;
	var req = new XMLHttpRequest();
	req.open('GET', "http://api.spitcast.com/api/spot/neighbors/" + Id + "/?direction=" + dir, false);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
//				if (DEBUG === true) console.log(req.responseText);
				response = JSON.parse(req.responseText);
				if (response && response.length > 0) {
					var len = response.length;
					if (dir === "above") {spot_id = response[len - 1].spot_id;}
					if (dir === "below") {spot_id = response[0].spot_id;}					
					if (DEBUG === true) console.log("Spot Id: " + spot_id);
				}
			}
			else {
				if (DEBUG === true) console.log("fetchNeighbor:HTTP " + req.status + " error");
			}
		};
		req.send(null);
		req.onloadend = function() {
			if (DEBUG === true) console.log("Done.");
			fetchCounty(spot_id);
		};
}

function fetchWater(county_name) {
	var response;
	if (DEBUG === true) console.log("Fetching Water Data for: " + county_name);
	var req = new XMLHttpRequest();
	req.open('GET', "http://api.spitcast.com/api/county/water-temperature/" + county_name, false);
	req.onload = function(e) {
		if (req.readyState == 4) {
			if (req.status == 200) {
//				if (DEBUG === true) console.log(req.responseText);
				response = JSON.parse("[" + req.responseText + "]");
				if (response && response.length > 0) {
					water_temp = response[0].fahrenheit;
					if (DEBUG === true) console.log("Water Temp: " + water_temp);
				}
			}
			else {
				if (DEBUG === true) console.log("fetchWater:HTTP " + req.status + " error");
			}
		}
	};
	req.send(null);
	req.onloadend = function() {
		if (DEBUG === true) console.log("Done.");
	};
}

function fetchSurf(Id, index) {
	var response;
	if (DEBUG === true) console.log("Fetching Surf for: Spot " + Id);
	var req = new XMLHttpRequest();
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
				response = JSON.parse(req.responseText);
				var spot_name, surf_hght, surf_text, hour_text, day_text, swel_text, wind_text, tide_text;
				if (response && response.length > 0) {
					var surfResult = response;
					if (index === "next_hr") hr++;
					if (index === "prev_hr") hr--;
					if (index === "next_day") hr += 24;
					if (index === "prev_day") hr -= 24;
					if (hr > 168) hr = 168;
					if (hr < Math.round(d.getMinutes()/60) + d.getHours()) hr = parseInt(Math.round(d.getMinutes()/60) + d.getHours());
					if (DEBUG) console.log("Hr: " + hr);
					
					spot_name = surfResult[hr].spot_name;
					surf_hght = surfResult[hr].size_ft;
					surf_text = surfResult[hr].shape_full;
					hour_text = surfResult[hr].hour;
					day_text  = surfResult[hr].day;
					swel_text = surfResult[hr].shape_detail.swell;
					wind_text = surfResult[hr].shape_detail.wind;
					tide_text = surfResult[hr].shape_detail.tide;
					
					if (DEBUG === true) console.log("Spot Name: " + spot_name);
					if (DEBUG === true) console.log("Surf: " + Math.floor(surf_hght) + "-" + Math.ceil(surf_hght) + " ft " + surf_text);
					if (DEBUG === true) console.log("Hour: " + hour_text + " " + day_text);
					if (DEBUG === true) console.log("Swell: "+ swel_text);
					if (DEBUG === true) console.log("Wind: " + wind_text);
					if (DEBUG === true) console.log("Tide: " + tide_text);
					
					// Send data to Pebble
					sendMessage({
						"spot_name": spot_name,
						"surf_text": Math.floor(surf_hght) + "-" + Math.ceil(surf_hght) + " ft " + surf_text,
						"time_text": hour_text + " " + day_text,
						"swel_text": "Swell: " + swel_text,
						"wind_text": "Wind: " + wind_text,
						"tide_text": "Tide: " + tide_text
					});
				}
			} 
			else {
				if (DEBUG === true) console.log("fetchSurf:HTTP " + req.status + " error!");
			}
	};
	req.onloadend = function () {
		if (DEBUG === true) console.log("Done.");
	};
	req.open('GET', "http://api.spitcast.com/api/spot/forecast/" + Id + "/?dcat=week", false); 
	req.send();
}

function sendMessage(message) {
	var delivered;
	Pebble.sendAppMessage(message, function(e) 
						{
							if (DEBUG === true) console.log("Successfully delivered; transactionId=" + e.data.transactionId);
							delivered = true;
						}, function(e) {
							if (DEBUG === true) console.log("Unable to deliver; transactionId="+e.data.transactionId);
							delivered = false;
						});
	return delivered;
}

function locationSuccess(pos) {
	if (DEBUG === true) console.warn("Location Found!");
//	var coordinates = pos.coords;
	hr = parseInt(Math.round(d.getMinutes()/60) + d.getHours());
	fetchNearby(pos);
}

function locationError(err) {
	if (DEBUG === true) console.warn("location error (" + err.code + "): " + err.message);
	if (err.code == 3) {
	}
	else if (err.code == 1) {
		sendMessage({
			"spot_name":"GPS DENIED"
		});
	}
		else {
			sendMessage({
				"spot_name":"Unavailable"
			});
		}
}

var locationOptions = {"timeout": 30000, "maximumAge": 60000 }; 


Pebble.addEventListener("ready",
						function(e) {
							if (DEBUG === true) console.log("connect!" + e.ready);
							if (DEBUG === true) console.log("Type: " + e.type);
							if (e.type == "ready") {
								// navigator.geolocation.watchPosition(locationSuccess, locationError, locationOptions);
								
		Pebble.addEventListener("appmessage",
								function(e) {
									if (DEBUG === true) console.log(e.type);
									if (DEBUG === true) console.log("Index: " + e.payload.index);
									if (DEBUG === true) console.log("Message: " + e.payload.index);
									var index = e.payload.index;
									if (index == "locate")
									{
										navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
									}
									else if (index == "return")
									{
										fetchSurf(nearby_spot_id, index);
									}
									else if (index == "next_hr" || index == "refresh" || index == "prev_hr" || index == "next_day" || index == "prev_day")
									{
										fetchSurf(spot_id, index);
									}
									else if (index == "above" || index == "below")
									{
										fetchNeighbor(spot_id, index);
										fetchSurf(spot_id, index);
									}
									});
							}
});