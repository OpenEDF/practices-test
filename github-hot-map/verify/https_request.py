# https request api github hot map
# python code is auto generate postman
import requests
import json

url = "https://api.github.com/graphql"

payload = json.dumps({
  "query": "query {user(login: \"OpenEDF\") {contributionsCollection(from: \"2021-06-18T00:00:01+08:00\", to: \"2021-07-22T23:59:59+08:00\") {contributionCalendar {weeks {contributionDays {color date contributionCount }}}}}}"
})
headers = {
  'Authorization': 'Bearer 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',
  'Content-Type': 'application/json',
  'Cookie': '_octo=GH1.1.616039335.1626847124; logged_in=no'
}

response = requests.request("POST", url, headers=headers, data=payload)

print(response.text)
