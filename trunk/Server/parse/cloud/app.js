//EXPRESS FRAMWORK


// These two lines are required to initialize Express in Cloud Code.
var express = require('express');
var app = express();

// Global app configuration section
app.set('views', 'cloud/views');  // Specify the folder to find templates
app.set('view engine', 'ejs');    // Set the template engine
app.use(express.bodyParser());    // Middleware for reading request body


//POST to http://wordswithking.parseapp.com/hello
app.post('/hello', function(req, res) {
	res.send("POST: Hello world! in app.js");
});

//GET to http://wordswithking.parseapp.com/hello
app.get('/hello', function(req, res) {
	res.send("GET: Hello world! in app.js");
});


//POST to http://wordswithking.parseapp.com/echo
app.post(
	'/echo', 
	function(req, res) {
		res.send(req.query.message);
	}
);


//GET to http://wordswithking.parseapp.com/findIndex
app.get(
	'/findIndex',
	function(req, res) {
		var startDate = new Date();
		
		var query = new Parse.Query('Words');
		query.equalTo('index', req.query.index);
		query.find({
			success: function(results) {
				var endDate = new Date();
				var delta = endDate - startDate;
				res.send('delta time: ' + delta + '</br> word: ' + results[0].get('english'));
			},
			error: function(error) {
				res.send('error');
			}
		});
		

	}
);

// // Example reading from the request query string of an HTTP get request.
// app.get('/test', function(req, res) {
//   // GET http://example.parseapp.com/test?message=hello
//   res.send(req.query.message);
// });

// // Example reading from the request body of an HTTP post request.
// app.post('/test', function(req, res) {
//   // POST http://example.parseapp.com/test (with request body "message=hello")
//   res.send(req.body.message);
// });

// Attach the Express app to Cloud Code.
app.listen();
