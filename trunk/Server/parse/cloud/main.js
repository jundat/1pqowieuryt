//ENTRY POINT

require('cloud/app.js');
require('cloud/match.js');
require('cloud/push.js');
require('cloud/user.js');
require('cloud/test.js');
require('cloud/chart.js');
require('cloud/money.js');


Parse.Cloud.define("hello", function(req, res) {
	res.success("Hello world!");
});

var listRoomType = [2, 5, 10, 15, 25];

Parse.Cloud.define("createRoomType", function(req, res) {
	for (var i = listRoomType.length - 1; i >= 0; i--) {
		var type = listRoomType[i];

		var RoomType = Parse.Object.extend('RoomType');
		var roomType = new RoomType();
		roomType.set('match', '');
		roomType.set('matchId', 0);
		roomType.set('type', type);

		roomType.save(null, {
			success: function(restul) {

			},
			error: function(error) {

			}
		});
	};
});

