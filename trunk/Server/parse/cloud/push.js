//PUSH NOTIFICATION FUNCTION


//OK
//push a messgae to all
//use to debug
Parse.Cloud.define("pushMessageToAll", function(req, res) {
	var everyoneQuery = new Parse.Query(Parse.Installation);

	Parse.Push.send({
		where: everyoneQuery,
	  	data: {
	  		alert: req.params.message
	  	}
		}, {
		  	success: function() {
				res.success("SUCCESS: " + req.params.message);
		  	},
		  	error: function(error) {
				res.error("ERROR: " + req.params.message);
		  	}
		}
	);
}); //end push all message to all


//OK
//Push match to all room in list(roomType)
function PushMatchToRoomType(roomType) {
	var query1 = new Parse.Query('RoomType');
	query1.equalTo('type', roomType);
	query1.first({
		success: function(result1) {
			var match = result1.get('match');

			var query2 = new Parse.Query('List' + roomType);
			query2.find({
				success: function(results2) {
					for(var i = results2.length - 1; i >= 0; i--) {			
						//push match to channel RoomName
						var roomName = results2[i].get('name');
						
						Parse.Push.send({
						  	channels: [ roomName ],
						  	data: {
						    	res : "pushMatch",
							  	data : JSON.parse(match)
						  	}
						}, {
						  	success: function() {},
						  	error: function(error) {}
						});
					}
				},
				error: function(error1) {}
			});

		},
		error: function(error) {}
	});
}


//OK
//Push new match to all room
Parse.Cloud.define('pushAllMatch',
	function(req, res) {
		var roomType = req.params.roomType;
		PushMatchToRoomType(roomType);
}); //end Push new match 


//OK
//Push a JSON object to channel
function PushData(channel, data) {
	Parse.Push.send({
	  	channels: [ channel ],
	  	data: data
	}, {
	  	success: function() {},
	  	error: function(error) {}
	});
}


var NOT_SUBMIT_TIME = -1; 
var NAMES_TABLE_SIZE = 200; //number of record in table Names


//OK
//Private
//Push result to 1 room type
function PushResultToRoomType (roomType) {
	var query1 = new Parse.Query('List' + roomType); //List all Room
	query1.find({
		success: function(results1) {

			//get matchId
			var queryMatchId = new Parse.Query('RoomType');
			queryMatchId.equalTo('type', roomType);
			queryMatchId.first({
				success: function(result2) {
					var matchId = result2.get('matchId') - 1;

					for(var i = results1.length - 1; i >= 0; i--) {	
						var roomName = results1[i].get('name');

						var query3 = new Parse.Query(roomName);
						query3.ascending('time');
						query3.greaterThan('time', NOT_SUBMIT_TIME); //all played user //OrEqualTo
						query3.find({
						  	success: function(results) {
						  		var listUser = []; //list all
						  		var LEN = results.length;
						  		for (var i = 0; i < LEN; i++) {
						  			listUser.push(results[i].get('username'));
						  		};

						  		//king info
						  		var kingUserName;
						  		var kingName;
						  		var kingTime;

						  		if(results[0] !== undefined) { //someone win------------------------------------
						  			kingUserName = results[0].get('username');
						  			kingName = results[0].get('name');
						  			kingTime = results[0].get('time');

							  		//push data
								  	var data = {
										res : "pushResult",
										data : {
											matchId: matchId,
											king: {
												username: kingUserName,
												name: kingName, 
												time: kingTime
											},
											list: listUser
										}
								  	}

			  						PushData(roomName, data);
			  						
			  						Parse.Cloud.useMasterKey('G5KFW0LVyy1c66uMkZIZzM8VZmxlXgIht3KLZ55a');//('91uRD0dbcFV9hmIkXgAaZVIlCb8QE0W3CdJikgPJ');

			  						//find out the king and update achivement
			  						var queryKing = new Parse.Query(Parse.User);
			  						queryKing.equalTo('username', kingUserName);
			  						queryKing.first({
		  								success: function(king) {
		  									var achi = king.get('achi' + roomType);
		  									king.set('achi' + roomType, achi + 1);
		  									king.save(null, {
		  										success: function(result) {},
												error: function(error) {}
		  									});
		  								},
		  								error: function(king, error) {
		  								}
		  							});

						  		} else { //no one win ----------------------------------------------------------
						  			//random king's info
						  			kingUserName = "robot";
						  			listUser[0] = kingUserName;
						  			kingTime = Math.floor(Math.random() * 10000 + 5000); //5s -> 10s
						  			//kingName
						  			var index = Math.floor(Math.random() * NAMES_TABLE_SIZE);
						  			var query3 = new Parse.Query('Names');
						  			query3.equalTo('index', index);
						  			query3.first({
					  					success: function(object1) {
					  						kingName = object1.get('name');
					  						var data = {
												res : "pushResult",
												data : {
													matchId: matchId,
													king: {
														username: kingUserName,
														name: kingName, 
														time: kingTime
													},
													list: listUser
												}
										  	}

					  						PushData(roomName, data);
					  					},
					  					error: function(object1, error) {}
					  				});
						  		} //----------------------------------------------------------------------------
						  	},
						  	error: function(error) {
						    	//res.error('Can not find all user in room: ' + roomName);
						  	}
						});
					}
				}, //----------------------------------------------------------------------
				error: function(error2) {}
			});
		},
		error: function(error1) {
			//res.error('Can not find all in List2');
		}
	}); //find in list room
} //end push result to 1 room type



//OK
//Push Result to all user
//Return list all user in room
Parse.Cloud.define('pushAllResult',
	function(req, res) {
		var roomType = req.params.roomType;
		PushResultToRoomType(roomType);
}); //end pushAllResult





