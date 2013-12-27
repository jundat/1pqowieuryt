//CHART

//number of top winner in chart
var NUMBER_TOP = 50;


//Get the top of most time winner
Parse.Cloud.define("getWinChart",
	function(req, res) {
		var roomType = req.params.roomType;
		var achiType = 'achi' + roomType;

		var query1 = new Parse.Query(Parse.User);
		query1.select('username');
		query1.select('name');
		query1.select(achiType);
		query1.descending(achiType);
		query1.limit(NUMBER_TOP); //TOP 50
		query1.find(
		{
			success: function(results) {
				var list = [];
				var LEN = results.length;
				for (var i = 0; i < LEN; i++) {
					var user = [
						results[i].get('username'),
						results[i].get('name'),
						results[i].get(achiType)
					]

					list.push(user);
				};

				var obj = {
					res: 'getWinChart' + roomType,
					data: {
						roomType: roomType,
						list: list
					}
				}

				res.success(JSON.stringify(obj));
			},
			error: function(error) {}
		});
	}
); //end getWinChart


//Get money chart of all user
Parse.Cloud.define("getMoneyChart",
	function(req, res) {
		var query1 = new Parse.Query(Parse.User);
		query1.select('username');
		query1.select('name');
		query1.select('money');
		query1.descending('money');
		query1.limit(NUMBER_TOP); //TOP 50
		query1.find(
		{
			success: function(results) {
				var list = [];
				var LEN = results.length;
				for (var i = 0; i < LEN; i++) {
					var user = [
						results[i].get('username'),
						results[i].get('name'),
						results[i].get('money')
					]

					list.push(user);
				};

				var obj = {
					res: 'getMoneyChart',
					data: {
						list: list
					}
				}

				res.success(JSON.stringify(obj));
			},
			error: function(error) {}
		});
	}
); //end getMoneyChart

