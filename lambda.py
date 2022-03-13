import json
import boto3

client = boto3.client('iot-data', region_name='us-east-1')

def lambda_handler(event, context):
    print(event)
    # TODO implement
    # Change topic, qos and payload
    response = client.publish(
        topic='cat/commands',
        qos=1,
        payload=json.dumps({"command":"f99"})
    )
    print(response)
    
    return {
        'statusCode': 200,
        'body': json.dumps('Published to topic')
    }
