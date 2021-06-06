var switchStatus_led1 = "OFF", switchStatus_led2 = "OFF", switchStatus_fan = "OFF", switchStatus_plug = "OFF";

const jsn = {
    'led1' : 'OFF',
    'led2' : 'OFF',
    'fan' : 'OFF',
    'plug' : 'OFF'
}
// Send connection message to flask using POST method
fetch('/control', {
    // Declare what type of data we're sending
    headers: {
    'Content-Type': 'application/json'
    },
    // Specify the method
    method: 'POST',
    // A JSON payload
    body: JSON.stringify({
        "greet": "[status] Web connection successful."
    })
    }).then(function (response) { 
        // At this point, Flask has printed our JSON
        return response.text();
        }).then(function (text) {

            console.log('Message Sent. ');
            console.log('response: ');
            // Should be 'OK' if everything was successful
            console.log(text);
    });

// Function to toggle the color of led when called.
function toggle_state(id)
{
    if (document.getElementById(id).style.color == "rgb(221, 221, 0)")
    {
        document.getElementById(id).style.color = "black";
        document.getElementById(id).style.transition = "0.5s";
        if (id == 'led1')
        {
            switchStatus_led1 = 'OFF';
            jsn.led1 = 'OFF';
        }
        else if (id == 'led2')
        {
            switchStatus_led2 = 'OFF';
            jsn.led2 = 'OFF';
        }
        else if (id == 'fan')
        {
            switchStatus_fan = 'OFF';
            jsn.fan = 'OFF';
        }
        else if (id == 'plug')
        {
            switchStatus_plug = 'OFF';
            jsn.plug = 'OFF';
        }
    }
    else
    {
        document.getElementById(id).style.color = "rgb(221, 221, 0)";
        document.getElementById(id).style.transition = "0.5s";
        if (id == 'led1')
        {
            switchStatus_led1 = 'ON';
            jsn.led1 = 'ON';
        }
        else if (id == 'led2')
        {
            switchStatus_led2 = 'ON';
            jsn.led2 = 'ON';
        }
        else if (id == 'fan')
        {
            switchStatus_fan = 'ON';
            jsn.fan = 'ON';
        }
        else if (id == 'plug')
        {
            switchStatus_plug = 'ON';
            jsn.plug = 'ON';
        }
    }
}

function update_switch(j)
{
    if (j.led1 == 'ON')
        {
            document.getElementById('led1').style.transition = "0.5s";
            document.getElementById('led1').style.color = "rgb(221, 221, 0)";
            
        }
    else
        {
            document.getElementById('led1').style.transition = "0.5s";
            document.getElementById('led1').style.color = "black";
            
        }
    
    if (j.led2 == 'ON')
        {
            document.getElementById('led2').style.color = "rgb(221, 221, 0)";
            document.getElementById('led2').style.transition = "0.5s";
        }
    else
        {
            document.getElementById('led2').style.color = "black";
            document.getElementById('led2').style.transition = "0.5s";
        }

    if (j.fan == 'ON')
        {
            document.getElementById('fan').style.color = "rgb(221, 221, 0)";
            document.getElementById('fan').style.transition = "0.5s";
        }
    else
        {
            document.getElementById('fan').style.color = "black";
            document.getElementById('fan').style.transition = "0.5s";
        }

    if (j.plug == 'ON')
        {
            document.getElementById('plug').style.color = "rgb(221, 221, 0)";
            document.getElementById('plug').style.transition = "0.5s";
        }
    else
        {
            document.getElementById('plug').style.color = "black";
            document.getElementById('plug').style.transition = "0.5s";
        }
}


var intervalId = window.setInterval(function(){
    // Send the same request
    fetch('/control')
        .then(function (response) {
            return response.json(); // But parse it as JSON this time
        })
        .then(function (json) {
            console.log('GET response as JSON:');
            console.log(json); // Here’s our JSON object
            
            switchStatus_led1 = json.led1;
            switchStatus_led2 = json.led2;
            switchStatus_fan = json.fan;
            switchStatus_plug = json.plug;

            update_switch(json)
        })

    // Send connection message to flask using POST method
fetch('/control', {
    // Declare what type of data we're sending
    headers: {
    'Content-Type': 'application/json'
    },
    // Specify the method
    method: 'POST',
    // A JSON payload
    body: JSON.stringify(jsn)
    }).then(function (response) { 
        // At this point, Flask has printed our JSON
        return response.text();
        }).then(function (text) {

            console.log('Message Sent. ');
            console.log('response: ');
            // Should be 'OK' if everything was successful
            console.log(text);
    });
    }, 5000);