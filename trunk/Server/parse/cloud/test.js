//TEST FUNCTIONS


var testCounter = 0;


Parse.Cloud.define("testCounter", function(req, res) {
	testCounter++;
	res.success(testCounter);
});


//return error
Parse.Cloud.define("funcError", function(req, res) {
	res.error("ERROR");
});


//return timeout
Parse.Cloud.define("funcTimeout", function(req, res) {
	var TOTAL = 20;
	var count = 0;
	var startDate = new Date();
	
	for(var i = 0; i <= TOTAL; ++i) {
		Parse.Cloud.run("get3Words",
			{},
			{
				success: function(result) {
					count++;
				},
				
				error: function(error) {
					count++;
				}
			}
		);
	}
	
	while(count < TOTAL) {
	}
	
	var endDate = new Date();
	var delta = endDate - startDate;
	var obj = 
	{
		total: TOTAL,
		message: "Success",
		time: delta
	}
	
	res.success(JSON.stringify(obj));
});

