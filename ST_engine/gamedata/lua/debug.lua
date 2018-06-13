--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

--log types
ERROR = 0x01
SUCCESS = 0x02
INFO = 0x04

function log(type, string)
    logLua(type, tostring(string))
end