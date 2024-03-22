const dotenv = require('dotenv');
const mysql = require("mysql2");

dotenv.config()
const pool = mysql.createPool({
    host: process.env.MYSQL_HOST,
    user: process.env.MYSQL_USER,
    database: process.env.MYSQL_DATABASE,
    password: process.env.MYSQL_PASSWORD,
    port: process.env.MYSQL_PORT
});

const promisePool = pool.promise();

async function updateDesktops(computerName) {
    try {
        const [rows] = await promisePool.query('SELECT * FROM DESKTOPS WHERE COMPUTERNAME = ?', [computerName]);

        if (rows.length === 0) {
          
            await promisePool.query('INSERT INTO DESKTOPS (COMPUTERNAME, BUTTONPRESSED, LAST_UPDATED) VALUES (?, ?, CURRENT_TIMESTAMP())', [computerName, 0]);
      
        } else {
       
            await promisePool.query('UPDATE DESKTOPS SET LAST_UPDATED =CURRENT_TIMESTAMP() WHERE COMPUTERNAME = ?', [computerName]);

        }
    } catch (error) {
        console.error('Error updating desktop:', error);
        throw error;
    }
}
async function checkUserExists(computerName) {
    try {
        const [rows] = await promisePool.query('SELECT * FROM DESKTOPS WHERE COMPUTERNAME = ?', [computerName]);
        return rows.length > 0; // Return true if user exists, false otherwise
    } catch (error) {
        console.error('Error checking user existence:', error);
        throw error;
    }
}

async function ChangeButtonCode(computerName, buttonCode) {
    try {
        const [rows] = await promisePool.query('UPDATE DESKTOPS SET BUTTONPRESSED = ? WHERE COMPUTERNAME = ?', [buttonCode, computerName]);
      
    } catch (error ) {
        console.error('Error updating button code:', error);
        throw error;
    }
}

async function getButtonCode(computerName) {
    try {
        const [rows] = await promisePool.query('SELECT BUTTONPRESSED FROM DESKTOPS WHERE COMPUTERNAME = ?', [computerName]);

        if(rows.length > 0) {
            return rows[0].BUTTONPRESSED;

        } else {
            return 0;
        }

    } catch (error ) {
        console.error('Error getting button code:', error);
        throw error;
    }
}

async function fetchActiveUsers() {
    try {
        // Fetch active users from the database
        const [rows] = await promisePool.query('SELECT COMPUTERNAME, LAST_UPDATED FROM DESKTOPS WHERE LAST_UPDATED > NOW() - INTERVAL 5 MINUTE');
        return rows;
    } catch (error) {
        console.error('Error fetching active users:', error);
        throw error;
    }
}

module.exports = { updateDesktops, getButtonCode, ChangeButtonCode, fetchActiveUsers, checkUserExists }