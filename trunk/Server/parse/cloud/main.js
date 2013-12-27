//ENTRY POINT

Parse.Cloud.define('testCloudFunction',
    function(req, res) {
        var name = req.params.name;
		
		var data = {
			username: name,
			password: 1234,
			address: "Tp. Ho Chi Minh"
		}

		res.success(data);

		//res.error("Failed");
});