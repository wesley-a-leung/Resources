#!/bin/sh

host=zaphod

java -cp /tmp/client \
     -Djava.security.policy=/tmp/policy/client.policy \
     -Dexamples.activation.client.codebase=file:/tmp/client/ \
     -Dexamples.activation.name=examples.activation.Counter \
     examples.activation.CounterClient $host
