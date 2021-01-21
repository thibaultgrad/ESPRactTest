import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { PodomaticState } from './types';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "lightState";

type PodomaticStateWebSocketControllerProps = WebSocketControllerProps<PodomaticState>;

class PodomaticStateWebSocketController extends Component<PodomaticStateWebSocketControllerProps> {

  render() {
    return (
      <SectionContent title='WebSocket Controller' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <PodomaticStateWebSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, PodomaticStateWebSocketController);

type PodomaticStateWebSocketControllerFormProps = WebSocketFormProps<PodomaticState>;

function PodomaticStateWebSocketControllerForm(props: PodomaticStateWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;

  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The switch below controls the LED via the WebSocket. It will automatically update whenever the LED state changes.
        </Typography>
      </Box>
      <Typography variant="body1">
    Etat en cours : {data.etat}
    </Typography>
    </ValidatorForm>
  );
}