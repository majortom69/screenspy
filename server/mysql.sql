CREATE TABLE DESKTOPS (
    COMPUTERNAME VARCHAR(20) PRIMARY KEY NOT NULL,
    BUTTONPRESSED INT,
    LAST_UPDATED TIMESTAMP DEFAULT CURRENT_TIMESTAMP()
);
