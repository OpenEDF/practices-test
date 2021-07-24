#!/bin/bash
script='query{
	user(login: \"OpenEDF\") {
    contributionsCollection(from: \"2021-06-18T00:00:01+08:00\", to: \"2021-07-18T23:59:59+08:00\") {
      		contributionCalendar {
        		weeks {
          			contributionDays {
            			color
            			date
            			contributionCount
          			}
        		}
      		}
    	}
  	}
}'

script="$(echo $script)"   # the query should be onliner, without newlines
curl --insecure -i -H 'Content-Type: application/json' \
   -H "Authorization: bearer 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" \
   -X POST -d "{ \"query\": \"$script\"}" https://api.github.com/graphql
