let MBED_ACCESS_KEY = 'YOUR_ACCESS_KEY_HERE';
let UA_KEY = 'YOUR_GA_ID_HERE (starts with UA)';

const ua = require('universal-analytics');
const MbedConnectorApi = require('mbed-connector-api');
const promisify = require('es6-promisify');


if (MBED_ACCESS_KEY === 'YOUR_ACCESS_KEY_HERE') MBED_ACCESS_KEY = process.env.TOKEN || '';
if (UA_KEY === 'YOUR_GA_ID_HERE (starts with UA)') UA_KEY = process.env.UA_KEY || '';

if (!MBED_ACCESS_KEY) {
    throw 'Set your mbed access key in ga-service/server.js';
}
if (!UA_KEY) {
    throw 'Set your GA access key in ga-service/server.js';
}

var connector = new MbedConnectorApi({ accessKey: MBED_ACCESS_KEY });

async function subscribeToGesture(endpoint) {
    await connector.putResourceSubscription(endpoint, '/gesture/0/value');
    console.log(endpoint, 'Subscribed to gesture');
}

(async function() {

    try {
        await connector.startLongPolling();
        let endpoints = await connector.getEndpoints({ parameters: { type: 'gesture-detector' } });

        console.log(`Connected... Got ${endpoints.length} endpoints`, endpoints);

        await Promise.all(endpoints.map(e => subscribeToGesture(e.name)));

        let visitors = endpoints.reduce((curr, e) => {
            curr[e.name] = ua(UA_KEY, e.name);
            return curr;
        }, {});

        console.log('Initialized GA');

        connector.on('notification', n => {
            let value;

            switch (Number(n.payload)) {
                // case 1: value = 'UP'; break;
                // case 2: value = 'DOWN'; break;
                // case 3: value = 'LEFT'; break;
                // case 4: value = 'RIGHT'; break;
                // case 5: value = 'FACE_UP'; break;
                // case 6: value = 'FACE_DOWN'; break;
                case 7: value = 'FREEFALL'; break;
                // case 8: value = '3G'; break;
                // case 9: value = '6G'; break;
                // case 10: value = '8G'; break;
                case 11: value = 'SHAKE'; break;
                // default: value = 'NONE'; break;
            }

            if (!value) return;

            // create a Google Analytics event
            visitors[n.ep].event("Gesture", value, function(err) {
                if (err) return console.error('Sending event to GA failed', err, n.ep, value);

                console.log('Sent event to GA', n.ep, value);
            });

        });

    }
    catch (ex) {
        console.error('Error...', ex);
    }

})();
