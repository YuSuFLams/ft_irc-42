#pragma once

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#define ERR_NEEDMOREPARAMS_111(hostname, nickUser, command) ( ":" + hostname +  " 461 " + nickUser + " " + command + " :Not enough parameters\r\n")

#define ERR_NOSUCHNICK_111(hostname, nickUser, nick) ( ":" + hostname +  " 401 " + nickUser + " " + nick + " :No such nick\r\n")

#define ERR_NOSUCHCHANNEL_111(hostname, nickUser, channel) ( ":" + hostname +  " 403 " + nickUser + " " + channel + " :No such channel\r\n")

#define ERR_NOTONCHANNEL_111(hostname, nickUser, channel) ( ":" + hostname +  " 442 " + nickUser + " " + channel + " :You're not on that channel\r\n")

#define ERR_CHANOPRIVSNEEDED_111(hostname, nickUser, channel) ( ":" + hostname +  " 482 " + nickUser + " " + channel + " :You're not channel operator\r\n")

#define ERR_USERONCHANNEL_111(hostname, nickUser, nick, channel) ( ":" + hostname +  " 443 " + nickUser + " " + nick + " " + channel + " :is already on channel\r\n")

#define RPL_INVITING 341

#define ERR_KEYSET_222(hostname, nickUser, command) ( ":" + hostname +  " 467 " + nickUser + " " + command + " :Channel key already set\r\n")

#define ERR_UNKNOWNMODE_222(hostname, nickUser, mode) ( ":" + hostname +  " 472 " + nickUser + " " + mode + " :is unknown mode char to me\r\n")

#define ERR_USERSDONTMATCH_222(hostname, nickUser) ( ":" + hostname +  "502 " + nickUser + " :Cannot change mode for other users\r\n")

#define RPL_BANLIST_222 367

#define ERR_UMODEUNKNOWNFLAG_222(hostname, nickUser) ( ":" + hostname +  "501 " + nickUser + " :Unknown MODE flag\r\n")

#define RPL_CHANNELMODEIS_222(hostname, nickUser, channel, mode) ( ":" + hostname +  "324 " + nickUser + " " + channel + " " + mode + "\r\n")

#define RPL_ENDOFBANLIST_222(hostname, nickUser, channel) ( ":" + hostname +  "368 " + nickUser + " " + channel + " :End of channel ban list\r\n")

#define RPL_UMODEIS_222(hostname, nickUser, mode) ( ":" + hostname +  "221 " + nickUser + " " + mode + "\r\n")

#endif