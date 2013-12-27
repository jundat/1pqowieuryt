//MONEY MANAGEMENT



//OK
//Add money for user, return money and achievement
Parse.Cloud.define("addMoney",
	function(req, res) {
		//run in administrator mode
		Parse.Cloud.useMasterKey('G5KFW0LVyy1c66uMkZIZzM8VZmxlXgIht3KLZ55a');
		var username = req.params.username;
		var addMoney = req.params.addMoney;
		var achi = req.params.achi;

		var query = new Parse.Query(Parse.User);
		query.equalTo('username', username);
		query.first({
		  	success: function(user1) {
		  		var currentMoney = user1.get('money');
		  		//update money
				user1.set('money', currentMoney + addMoney);

				if(achi != "") {
					user1.set('achi', achi);
				}
				
		  		user1.save(null, {
		  			success: function(user2) {
		  				//return to client
						var obj = {
							res : "addMoney",
							data : {
								money : user2.get('money'),
								achi : user2.get('achi')
							}
						}

						res.success(JSON.stringify(obj));
		  			},
		  			error: function(error) {
		  				res.error("error in save back: " + JSON.stringify(error));
		  			}
		  		});			
			},
			error: function(user, error) {
				res.error('Add money error: ' + JSON.stringify(error));
			}
		});
}); //end add money

