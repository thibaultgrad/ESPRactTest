import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box,TextField, FormControlLabel, Switch} from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import RefreshIcon from '@material-ui/icons/Refresh';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent} from '../components';

import { SavedDataState } from './types';
import SettingsDataStateRestControllerForm from'./SettingsDataStateRestController' ;
//import { TextFieldsOutlined } from '@material-ui/icons';

export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "SavedDataState";

type SavedDataStateRestControllerProps = RestControllerProps<SavedDataState>;

class SavedDataStateRestController extends Component<SavedDataStateRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='Données podomatic' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <SavedDataStateRestControllerForm {...props} />
          )}
        />

      </SectionContent>
    )
  }

}

export default restController(LIGHT_SETTINGS_ENDPOINT, SavedDataStateRestController);

type SavedDataStateRestControllerFormProps = RestFormProps<SavedDataState>;

function refreshPage() {
  window.location.reload(false);
}

function SavedDataStateRestControllerForm(props: SavedDataStateRestControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          Historique
        </Typography>
      </Box>
      <Typography variant="subtitle2" gutterBottom>
            Temps total de spray en h : {data.temps_total_spray}
      </Typography>
      <Typography variant="subtitle2" gutterBottom>
            Nombre total de passages : {data.nb_total_passage}
      </Typography>
      <FormActions>
        <FormButton startIcon={<RefreshIcon />} variant="contained" color="primary" onClick={refreshPage}>
          Rafraichir
        </FormButton>
      </FormActions>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
        Reglage des parametres :
        </Typography>
      </Box>


    </ValidatorForm>
  );
}
