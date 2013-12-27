//USER MANAGERMENT FUNCTIONS
 
 
//maximun people in 1 room
var ROOM_SIZE_FACTOR = 10;
 
 
//OK
//Sign in
Parse.Cloud.define("signIn",
    function(req, res) {
        var username    = "u";
        var password    = req.params.password;
        var name        = req.params.name;
         
        //generate unique username
        var query1 = new Parse.Query(Parse.User);
        query1.count({
            success: function(total) {
                username += total; //user count from 0
                 
                //sign in new user
                var u = new Parse.User();
                u.set("username", username);
                u.set("password", password);
                u.set("name", name);
                u.set("fbId", "");
                u.set("money", 0);
                u.set("achi", "111111111"); //string
                u.set("achi2", 0); //how many time user win
                u.set("achi5", 0);
                u.set("achi10", 0);
                u.set("achi15", 0);
                u.set("achi25", 0);
                 
                //Sing up
                u.signUp(null, {
                    success: function(user) {
                        //return to client
                        var obj = 
                        {
                            res : "signin",
                            data :
                            {
                                username : username,
                                money : user.get('money'),
                                achi : user.get('achi')
                            }
                        }
                     
                        res.success(JSON.stringify(obj));
                    },
                    error: function(user, error) {
                        res.error("SignIn error: " + JSON.stringify(error));
                    }
                });
            }
        });
}); //end Sign in
 
 
//OK
//Log in
Parse.Cloud.define("logIn",
    function(req, res) {
        var username = req.params.username;
        var password = req.params.password;
         
        //log in
        Parse.User.logIn(username, password, {
            success: function(user) {
                //return to client
                var obj = 
                {
                    res : "login",
                    data : 
                    {
                        money : user.get('money'),
                        achi : user.get('achi')
                    }
                }
             
                res.success(JSON.stringify(obj));
            },
            error: function(user, error) {
                res.error('Sign in error');
            }
        });
    }
); //end Log in
 
 
//OK
//NOT NEED
//Log out
Parse.Cloud.define("logOut",
    function(req, res) {
        res.success('Log out success!');
    }
); //end Log out
 
 
 
//OK
//time: -2
var NEW_USER_TIME = -2;
 
 
 
//Choose room type
Parse.Cloud.define("chooseRoomType",
    function(req, res) {
        var username =  req.params.username;
        var playername = req.params.name;
        var roomType = req.params.roomType;
        var strList = 'List' + roomType;
        var NUMBER_ROOM_IN_LIST;
 
        //
        var query1 = new Parse.Query(strList);
        query1.find({
            success: function(results1) {
                NUMBER_ROOM_IN_LIST = results1.length;
 
                if(NUMBER_ROOM_IN_LIST == 0) { //create NEW ROOM-----------------------------------
 
                    //create new room in list room
                    var strNewRoom = 'List' + roomType + 'Room' + (NUMBER_ROOM_IN_LIST + 1);
                    var List = Parse.Object.extend(strList);
                    var objInList = new List();
 
                    objInList.set('name', strNewRoom);
                    objInList.set('numberOfUser', 1);
                    objInList.set('matchId', 0); //null
                    objInList.set('match', ''); //null
                    objInList.save(null, {
                            success: function(object1) {
                                //create new room with that name
                                var Room = Parse.Object.extend(strNewRoom);
                                var user = new Room();
                                user.set('name', playername);
                                user.set('username', username);
                                user.set('time', NEW_USER_TIME);
 
                                user.save(null, {
                                    success: function(object2) {
                                        //return to client
                                        var obj = 
                                        {
                                            res : "chooseRoomType", 
                                            data : 
                                            {
                                                roomName : strNewRoom,
                                                user: object2
                                            }
                                        }
 
                                        res.success(JSON.stringify(obj));
                                    },
                                    error: function(object2, error) {
                                        res.error('Can not save a user in new room');
                                    }
                                });
                            },
                            error: function(object1, error) {
                                res.error('Can not save in list');
                            }
                        }
                    );
                } else { //foreach list room  and check number of user in each room
                    //list room is results1
                    var hasRoom = false;
 
                    for (var i = results1.length - 1; i >= 0; i--) {
                        var roomName = results1[i].get('name');
                        var numberOfUser = results1[i].get('numberOfUser');
 
                        if(numberOfUser < roomType * ROOM_SIZE_FACTOR) {
                            hasRoom = true;
                            var obj =  {
                                res : "chooseRoomType", 
                                data : {
                                    roomName : roomName,
                                    user: playername
                                }
                            }
 
                            //add to room
                            //check if it had
                            var query3 = new Parse.Query(roomName);
                            query3.equalTo('username', username);
                            query3.count(
                            {
                                success: function(number1) {
                                    if(number1 == 0) { // ADD NEW
                                        var Room = Parse.Object.extend(roomName);
                                        var user = new Room();
                                        user.set('name', playername);
                                        user.set('username', username);
                                        user.set('time', NEW_USER_TIME);
 
                                        user.save(null, {
                                            success: function(object2) {
                                                //update numberOfUser in room
                                                results1[i].set('numberOfUser', numberOfUser + 1);
                                                results1[i].save();
 
                                                res.success(JSON.stringify(obj));
                                            },
                                            error: function(object2, error) {
                                                res.error('Can not save a user in new room');
                                            }
                                        });
                                    } else {
                                        res.success(JSON.stringify(obj));
                                    }
                                },
                                error: function(error) {
                                    res.error('Can not count in room: ' + roomName);
                                }
                            });
                            break;
                        }
                    };
 
                    if(hasRoom == false) { //NO room can add user
                        //add a new room
                        //continue here
                        //
                        //create new room in list room ------------------------- NEW ROOM
                        var strNewRoom = 'List' + roomType + 'Room' + (NUMBER_ROOM_IN_LIST + 1);
                        var List = Parse.Object.extend(strList);
                        var objInList = new List();
 
                        objInList.set('name', strNewRoom);
                        objInList.set('numberOfUser', 1);
                        objInList.set('matchId', 0); //null
                        objInList.set('match', ''); //null
                        objInList.save(null, {
                                success: function(object1) {
                                    //create new room with that name
                                    var Room = Parse.Object.extend(strNewRoom);
                                    var user = new Room();
                                    user.set('name', playername);
                                    user.set('username', username);
                                    user.set('time', NEW_USER_TIME);
 
                                    user.save(null, {
                                        success: function(object2) {
                                            //return to client
                                            var obj = 
                                            {
                                                res : "chooseRoomType", 
                                                data : 
                                                {
                                                    roomName : strNewRoom,
                                                    user: object2
                                                }
                                            }
 
                                            res.success(JSON.stringify(obj));
                                        },
                                        error: function(object2, error) {
                                            res.error('Can not save a user in new room');
                                        }
                                    });
                                },
                                error: function(object1, error) {
                                    res.error('Can not save in list');
                                }
                            }
                        );
                    }
                }
            },
 
            error: function(error) {
                res.error('Can not query rooms in list');
            }
        });
 
    }
); //end Choose room type
 
 
//OK
//Leave room
Parse.Cloud.define('leaveRoom',
    function(req, res) {
        var username = req.params.username;
        var roomName = req.params.roomName;
 
        var query = new Parse.Query(roomName);
        query.equalTo('username', username);
        query.first(
        {
            success: function(myObject) {
                myObject.destroy({ //delete user in room
                    success: function(myObject) {
                        //update to list room
                        var strList = roomName.substring(0, roomName.indexOf('Room'));
                        var query2 = new Parse.Query(strList);
                        query2.equalTo('name', roomName);
                        query2.first(
                        {
                            success: function(object) {
                                var numberOfUser = object.get('numberOfUser');
                                object.set('numberOfUser', numberOfUser - 1);
                                object.save();
                                res.success('OK');
                            },
                            error: function(error) {
                                res.error('Can not find room in list room');
                            }
                        });
                    },
                    error: function(myObject, error) {
                        res.error('Can not leave room: ' + roomName);
                    }
                });
            },
            error: function(error) {
                res.error('Can not find out username: ' + username);
            }
        });
    }
); //end leave room
 
 
//OK
//Submit
Parse.Cloud.define('submit',
    function(req, res) {
        var username = req.params.username;
        var roomName = req.params.roomName;
        var time = req.params.time;
        var word = req.params.word;
         
        var query = new Parse.Query(roomName);
        query.equalTo('username', username);
        query.first(
        {
            success: function(object) {
                object.set('time', time);
                object.save();
                res.success('OK');
            },
            error: function(error) {
                res.error('Can not find username: ' + username);
            }
        });
    }
); //end submit
 
 
//OK
//USE NEW USER TIME
//Private
//Refresh all result in list room (room type)
function RefreshResultInRoomType(roomType) {
    var query1 = new Parse.Query('List' + roomType);
    query1.find({
        success: function(results1) {
            //list room
            for(var i = results1.length - 1; i >= 0; i--) {
                var roomName = results1[i].get('name');
                var query2 = new Parse.Query(roomName);
                query2.find({
                    success: function(results2) {
                        for (var i = results2.length - 1; i >= 0; i--) {
                            results2[i].set('time', NEW_USER_TIME);
                        };
 
                        Parse.Object.saveAll(results2, {
                            success: function(list) {},
                            error: function(error) {},
                        });
                    },
                    error: function(error) {
                        //res.error('Can not find all user in room: ' + roomName);
                    }
                });
            }
        },
        error: function(error1) {
            //res.error('Can not find all in List2');
        }
    });
}
 
 
//OK
//Refresh all user's time in room
Parse.Cloud.define('refreshAllResult',
    function(req, res) {
        var roomType = req.params.roomType;
        RefreshResultInRoomType(roomType);
});