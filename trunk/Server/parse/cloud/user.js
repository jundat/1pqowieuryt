//USER MANAGERMENT FUNCTIONS

var NUMBER_TOP = 10;


//Sign in
Parse.Cloud.define("signIn",
    function(req, res) {
        var password    = req.params.password;
        var name        = req.params.name;
        var username    = "user";
         
        //generate unique username
        var query1 = new Parse.Query(Parse.User);
        query1.count({
            success: function(total) {
                username += total; //user count from 0
                 
                //sign in new user
                var newUser = new Parse.User();
                newUser.set("username", username);
                newUser.set("password", password);
                newUser.set("name", name);
                newUser.set("score", 0);
                 
                //Sing up
                newUser.signUp(null, {
                    success: function(user) {
                    	var result = '|' + username + '|';
                        res.success(result);
                    },
                    error: function(user, error) {
                        res.error(error);
                    }
                });
            }
        });
    }
); //end Sign in


//Log in
Parse.Cloud.define("logIn",
    function(req, res) {
        var username = req.params.username;
        var password = req.params.password;
         
        //log in
        Parse.User.logIn(username, password, {
            success: function(user) {
				var score = user.get('score');
				var result = '|' + score + '|';
                
                res.success(result);
            },
            error: function(user, error) {
                res.error(error);
            }
        });
    }
); //end Log in


//Submit
Parse.Cloud.define('submitScore',
    function(req, res) {
    	Parse.Cloud.useMasterKey('VUwBOXV3DCmZZaS28M1GYgZopdHvwepjPPU3JV6D');
        var username = req.params.username;
        var score = req.params.score;
        
        var query = new Parse.Query(Parse.User);
        query.equalTo('username', username);
        query.first(
        {
            success: function(user) {
                user.set('score', score);
                user.save();
                res.success('|' + score + '|');
            },
            error: function(error) {
                res.error(error);
            }
        });
    }
); //end submit


//Get Leaderboard
Parse.Cloud.define("getLeaderboard",
	function(req, res) {
		var query1 = new Parse.Query(Parse.User);
		query1.select('username');
		query1.select('name');
		query1.select('score');
		query1.descending('score');
		query1.limit(NUMBER_TOP); //TOP 10
		query1.find(
		{
			success: function(results) {
				var list = [];
				var LEN = results.length;

				for (var i = 0; i < LEN; i++) {
					var user = {
                        username: results[i].get('username'),
                        name: results[i].get('name'),
                        score: results[i].get('score')
                    };

					list.push(user);
				};

				res.success(list);
			},
			error: function(error) {
				res.error(error);
			}
		});
	}
); //end Get Leaderboard